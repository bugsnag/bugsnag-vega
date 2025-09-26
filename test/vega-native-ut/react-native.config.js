module.exports = {
    dependency: {
        platforms: {
            kepler: {
                "autolink": {
                    "BugsnagNativeUT": {
                        "libraryName": "libBugsnagNativeUT.so",
                        "linkDynamic": true,
                        "provider": "application",
                        "components": [],
                        "turbomodules": [
                            "BugsnagNativeUT"
                        ]
                    }
                }
            },
        },
    },
};