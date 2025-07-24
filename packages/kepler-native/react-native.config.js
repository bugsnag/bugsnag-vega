module.exports = {
    dependency: {
        platforms: {
            kepler: {
                "autolink": {
                    "BugsnagKeplerNative": {
                        "libraryName": "libBugsnagKeplerNative.so",
                        "linkDynamic": true,
                        "provider": "application",
                        "components": [],
                        "turbomodules": [
                            "BugsnagKeplerNative"
                        ]
                    }
                }
            },
        },
    },
};