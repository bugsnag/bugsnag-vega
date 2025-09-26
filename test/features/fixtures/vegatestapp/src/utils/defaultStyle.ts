import { StyleSheet } from "react-native";

export const getStyles = () =>
StyleSheet.create({
    background: {
        color: 'white',
        flex: 1,
        flexDirection: 'column',
    },
    headerContainer: {
        marginLeft: 200,
    },
    headerText: {
        color: 'white',
        fontSize: 80,
        marginBottom: 10,
    },
});
