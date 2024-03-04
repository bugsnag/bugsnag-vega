import { AsyncStorage } from "@amzn/react-native-kepler"

const LAST_COMMAND_KEY = "lastCommandUUID"

export async function setLastCommandUUID(uuid: string) {
    try {
        await AsyncStorage.setItem(LAST_COMMAND_KEY, uuid)
    } catch (error) {
        throw new Error('[Bugsnag] Could not save last command uuid in storage')
    }
}

export async function getLastCommandUUID() {
    try {
        const value = await AsyncStorage.getItem(LAST_COMMAND_KEY)
        return value
    } catch (error) {
        throw new Error('[Bugsnag] Could not read last command uuid from storage')
    }
}

export async function clearLastCommandUUID() {
    try {
        await AsyncStorage.removeItem(LAST_COMMAND_KEY)
    } catch (error) {
        throw new Error('[Bugsnag] Could not remove last command uuid from storage')
    }
}
