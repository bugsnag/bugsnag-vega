When('I launch the app') do
  Maze::Runner.run_command("kepler device launch-app -d Simulator -a com.bugsnag.fixtures.keplertestapp.main")   
end
