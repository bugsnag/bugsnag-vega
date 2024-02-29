import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"

export default {
    config: {},
    App: () => {
        useEffect(() => {
            Bugsnag.notify(new Error('HandledJSError'))
        }, [])
    
        return (
            <View>
                <Text>HandledJsError</Text>
            </View>
        )
    }
}
