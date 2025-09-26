module.exports = {
  dependency: {
    platforms: {
      kepler: {
        autolink: {
          BugsnagVegaNative: {
            libraryName: 'libBugsnagVegaNative.so',
            linkDynamic: true,
            provider: 'application',
            components: [],
            turbomodules: [
              'BugsnagVegaNative'
            ]
          }
        }
      }
    }
  }
}
