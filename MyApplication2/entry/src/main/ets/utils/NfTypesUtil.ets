import { List } from '@kit.ArkTS'
// 用于定义一些类型，避免重复声明

// 空类型
export type NfNull = null | undefined

//可空类型
export type NType<T> = T | NfNull

//数组或者是集合
export type NFList<T> = List<T> | Array<T>

// 无参数返回一个R
export type Function0<R> = () => R

// 传入一个R参数，返回一个R
export type Function1<R> = (r: R) => R

// 传入一个T参数，返回一个R
export type Function2<T, R> = (t: T) => R

//任意类型
export type Any = object | string | number | boolean | undefined

//string类型
export function isString(type: object | NfNull) {
  return typeof type === "string"
}

//number类型
export function isNum(type: object | NfNull) {
  return typeof type === "number"
}