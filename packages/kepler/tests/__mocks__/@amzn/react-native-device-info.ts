const DeviceInfo = {
  getManufacturerSync: jest.fn().mockReturnValue('Amazon'),
  getModel: jest.fn().mockReturnValue('Tv Simulator'),
  getSystemName: jest.fn().mockReturnValue('Kepler'),
  getSystemVersion: jest.fn().mockReturnValue('1.1'),
  getBundleId: jest.fn().mockReturnValue('com.bugsnag.fixtures.keplertestapp')
}

export default DeviceInfo
