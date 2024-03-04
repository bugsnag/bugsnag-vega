BeforeAll do
  Maze.config.enforce_bugsnag_integrity = false

  # start the simulator
  Maze::Runner.run_command("kepler device simulator start")

  # install the app
  Maze::Runner.run_command("kepler device uninstall-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
  Maze::Runner.run_command("kepler device install-app -d Simulator --dir features/fixtures/keplertestapp")
end

AfterAll do
  # stop the simulator
  Maze::Runner.run_command("kepler device simulator stop")
end

Maze.hooks.before do
  # launch the app
  Maze::Runner.run_command("kepler device launch-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
end

Maze.hooks.after do
  # terminate the app
  Maze::Runner.run_command("kepler device terminate-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")
end
