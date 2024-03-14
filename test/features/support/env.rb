BeforeAll do
  Maze.config.enforce_bugsnag_integrity = true

  # start the simulator
  Maze::Runner.run_command("kepler device simulator start")

  # start the simulator log stream
  Maze::Runner.run_command("kepler device start-log-stream -d Simulator > device.log", blocking: false)

  # install the app
  Maze::Runner.run_command("kepler device uninstall-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
  Maze::Runner.run_command("kepler device install-app -d Simulator --dir features/fixtures/keplertestapp")
end

AfterAll do
  # stop the simulator
  Maze::Runner.run_command("kepler device simulator stop")

  # move device logs to maze_output
  Maze::Runner.run_command("mv device.log maze_output")
end

Maze.hooks.before do
  # launch the app
  Maze::Runner.run_command("kepler device launch-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
end

Maze.hooks.after do
  # terminate the app
  Maze::Runner.run_command("kepler device terminate-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
end

Before do
  $address = nil
end
