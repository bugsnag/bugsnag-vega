import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import { NavigationContainer } from "@amazon-devices/react-navigation__native"
import { createStackNavigator } from "@amazon-devices/react-navigation__stack"
import BugsnagPluginReactNavigation from "@bugsnag/vega-plugin-react-navigation"
import React, { useEffect } from 'react'
import { Text, View } from 'react-native'
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'


const config: Partial<VegaConfig> = {
    enabledBreadcrumbTypes: [],
    plugins: [new BugsnagPluginReactNavigation()]
}

const Stack = createStackNavigator()
const styles = getStyles()

const App = () => {
    useEffect(() => {
        console.log("[bugsnag] Navigation scenario")
    }, [])

    const BugsnagNavigationContainer = Bugsnag.getPlugin('reactNavigation')!.createNavigationContainer(NavigationContainer)
    return (
        <BugsnagNavigationContainer>
            <Stack.Navigator initialRouteName='Screen1'>
                <Stack.Screen name='Screen1' component={Screen1} />
                <Stack.Screen name='Screen2' component={Screen2} />
                <Stack.Screen name='Screen3' component={Screen3} />
            </Stack.Navigator>
        </BugsnagNavigationContainer>
    )
}

const Screen1 = ({ navigation }: { navigation: any }) => {
    useEffect(() => {
        (async () => {
            await delay(1000)
            Bugsnag.notify('Error on Screen1')
            navigation.navigate('Screen2')
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>Screen1</Text>
            </View>
        </View>
    )
}

const Screen2 = ({ navigation }: { navigation: any }) => {
    useEffect(() => {
        (async () => {
            await delay(1000)
            Bugsnag.notify('Error on Screen2')
            navigation.navigate('Screen3')
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>Screen2</Text>
            </View>
        </View>
    )
}

const Screen3 = () => {
    useEffect(() => {
        (async () => {
            await delay(1000)
            Bugsnag.notify(new Error('NaviBreadcrumbError'))
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>Screen3</Text>
            </View>
        </View>
    )
}

export default { App, config }