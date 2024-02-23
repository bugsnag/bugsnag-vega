/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates.  All rights reserved.
 *
 * PROPRIETARY/CONFIDENTIAL.  USE IS SUBJECT TO LICENSE TERMS.
 */

import React from 'react';
import {StyleSheet, Text, View} from 'react-native';

export const App = () => {

  const styles = getStyles();

  return (
      <View style={styles.background}>
        <View style={styles.headerContainer}>
          <Text style={styles.headerText}>Bugsnag Kepler Test App</Text>
        </View>
      </View>
  );
};

const getStyles = () =>
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
