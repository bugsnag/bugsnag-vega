When('I launch the app') do
  Maze::Runner.run_command("kepler device launch-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")   
end

When('I run {string}') do |scenario_name|
  execute_command 'run-scenario', scenario_name
end

When('I execute the command {string}') do |command|
  execute_command(command)
end

def execute_command(action, scenario_name = '')
  address = case Maze::Helper.get_current_platform
              when 'android'
                'localhost:9339'
              else
                'bs-local.com:9339'
            end

  command = {
    action: action,
    scenario_name: scenario_name,
    endpoint: "http://#{address}/traces",
    api_key: $api_key,
  }

  $logger.debug("Queuing command: #{command}")
  Maze::Server.commands.add command

  # Ensure fixture has read the command
  count = 900
  sleep 0.1 until Maze::Server.commands.remaining.empty? || (count -= 1) < 1
  raise 'Test fixture did not GET /command' unless Maze::Server.commands.remaining.empty?
end
