$simulator = ENV['SIMULATOR'] || ENV['ON_DEVICE'].nil?

BeforeAll do
  Maze.config.enforce_bugsnag_integrity = true

  if $simulator
    # start the simulator
    Maze::Runner.run_command("kepler device simulator start")

    # start the simulator log stream
    Maze::Runner.run_command("kepler device start-log-stream -d Simulator > device.log", blocking: false)
  else
    # start the device log stream
    Maze::Runner.run_command("kepler device start-log-stream > device.log", blocking: false)
  end
end

AfterAll do
  # stop the simulator
  Maze::Runner.run_command("kepler device simulator stop") if $simulator

  # move device logs to maze_output
  Maze::Runner.run_command("mv device.log maze_output")
end

Maze.hooks.before do
  # reset the app, then launch it
  Maze::Runner.run_command("kepler device uninstall-app --appName com.bugsnag.fixtures.keplertestapp.main")

  if $simulator
    Maze::Runner.run_command("kepler device install-app -p ./features/fixtures/keplertestapp/build/vega-tv2023-aarch64-release/keplertestapp_aarch64.vpkg -d Simulator")
  else
    Maze::Runner.run_command("kepler device install-app -p ./features/fixtures/keplertestapp/build/vega-tv2023-armv7-release/keplertestapp_armv7.vpkg")
  end

  Maze::Runner.run_command("kepler device launch-app -a com.bugsnag.fixtures.keplertestapp.main")
end

After do
  # Terminate the app
  Maze::Runner.run_command("kepler device terminate-app --appName com.bugsnag.fixtures.keplertestapp.main")

  # Only copy files from the simulator
  if $simulator
    Maze::Runner.run_command("kepler device run-cmd --command 'rm -r /home/app_user/packages/com.bugsnag.fixtures.keplertestapp/data/bugsnag'")
  end

  # Uninstall the app
  Maze::Runner.run_command("kepler device uninstall-app --appName com.bugsnag.fixtures.keplertestapp.main")
end

Before do
  $address = nil
end

After('@native_unit_tests') do
  # Only copy files from the simulator
  if $simulator
    Maze::Runner.run_command("kepler device copy-from --source /home/app_user/packages/com.bugsnag.fixtures.keplertestapp/data/bugsnag/utOutput.txt --destination maze_output/")
  end
end
