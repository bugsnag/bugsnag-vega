Code for base BugsnagVegaNative turbomodule generated using command:
```
npx keplerscript-turbomodule-api codegen src/turbo-modules/BugsnagVegaNative.ts -o kepler/turbo-modules --new --namespace bugsnag
```

and for BugsnagFileIO turbomodule:
```
npx keplerscript-turbomodule-api codegen src/turbo-modules/BugsnagFileIO.ts -o kepler/turbo-modules --new --namespace bugsnag
```

The `--new` option is responsible for creating implementation files.
If it's used then new, empty implementation will be prepared and previous file renamed to `*.bak`.
If this option is omitted then only the `generated/*Spec` files will be created.