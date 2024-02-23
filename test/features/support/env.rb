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
