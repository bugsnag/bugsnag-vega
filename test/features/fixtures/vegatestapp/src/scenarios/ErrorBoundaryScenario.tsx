import Bugsnag, { type Event } from '@bugsnag/vega'
import React from 'react'
import { Text, View, Button } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {}

interface ErrorViewProps {
    clearError: () => void
}

const ErrorView = ({ clearError }: ErrorViewProps) =>
<View>
    <Text>Inform users of an error in the component tree.
    Use clearError to reset ErrorBoundary state and re-render child tree.</Text>
    <Button onPress={clearError} title="Reset" />
</View>

const onError = (event: Event) => {
    console.log('ErrorBoundary onError')
}

const BadComponent = () => {
    throw new Error('render error')
}

const App = () => {
    const styles = getStyles()

    const ErrorBoundary = Bugsnag.getPlugin('react').createErrorBoundary(React)

    return (
        <ErrorBoundary FallbackComponent={ErrorView} onError={onError}>
            <View style={styles.background}>
                <View style={styles.headerContainer}>
                    <Text style={styles.headerText}>ErrorBoundaryScenario</Text>
                    <BadComponent />
                </View>
            </View>
        </ErrorBoundary>
    )
}

export default { App, config }
