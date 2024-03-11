import {type ErrorResult} from "./turbo-modules/BugsnagFileIO"

export function isErrorResult(v: unknown): v is ErrorResult {
  return typeof v === 'object' && v != null &&
    (v as Partial<ErrorResult>).error === true &&
    typeof (v as Partial<ErrorResult>).message === 'string' &&
    typeof (v as Partial<ErrorResult>).code === 'number'
}
