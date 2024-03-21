When('I run {string}') do |scenario_name|
  execute_command 'run-scenario', scenario_name
end

When('I start bugsnag for {string}') do |scenario_name|
  execute_command 'start-bugsnag', scenario_name
end

When('I execute the command {string}') do |command|
  execute_command(command)
end

When('I configure the endpoints to {string}') do |address|
  $address = address
end

When('I configure the endpoints to default') do
  $address = nil
end

When('I restart the test fixture') do
  Maze::Runner.run_command("kepler device terminate-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
  Maze::Runner.run_command("kepler device launch-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
end

Then("the exception {string} equals one of:") do |keypath, possible_values|
  value = Maze::Helper.read_key_path(Maze::Server.errors.current[:body], "events.0.exceptions.0.#{keypath}")
  Maze.check.include(possible_values.raw.flatten, value)
end

def execute_command(action, scenario_name = '')
  address = $address ? $address : '10.0.2.2:9339'

  command = {
    action: action,
    scenario_name: scenario_name,
    notify_endpoint: "http://#{address}/notify",
    sessions_endpoint: "http://#{address}/sessions",
    api_key: $api_key,
  }

  $logger.debug("Queuing command: #{command}")
  Maze::Server.commands.add command

  # Ensure fixture has read the command
  count = 900
  sleep 0.1 until Maze::Server.commands.remaining.empty? || (count -= 1) < 1
  raise 'Test fixture did not GET /command' unless Maze::Server.commands.remaining.empty?
end
