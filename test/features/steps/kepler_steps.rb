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
  Maze::Runner.run_command("kepler device terminate-app -a com.bugsnag.fixtures.vegatestapp.main")
  Maze::Runner.run_command("kepler device launch-app -a com.bugsnag.fixtures.vegatestapp.main")
end

When('I print error folder contents') do
  # Only on the simulator
  if $simulator
    Maze::Runner.run_command("kepler device run-cmd --command 'ls -la /home/app_user/packages/com.bugsnag.fixtures.vegatestapp/data/bugsnag/errors'")
  end
end

When('I copy error file') do
  # Only on the simulator
  if $simulator
    Maze::Runner.run_command("kepler device copy-from -d Simulator --source /home/app_user/packages/com.bugsnag.fixtures.vegatestapp/data/bugsnag/errors/* --destination maze_output/")
  end
end

Then("the exception {string} equals one of:") do |keypath, possible_values|
  value = Maze::Helper.read_key_path(Maze::Server.errors.current[:body], "events.0.exceptions.0.#{keypath}")
  Maze.check.include(possible_values.raw.flatten, value)
end

def execute_command(action, scenario_name = '')
  if $simulator
    address = $address ? $address : '10.0.2.2:9339'
  else
    address = $address ? $address : "#{local_ip}:9339"
  end

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

def local_ip
  broadcast = `ifconfig | grep broadcast`.strip
  address = /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}).+broadcast/.match(broadcast)
  if address.nil?
    raise 'Could not find local IP address'
  end
  address[1]
end
