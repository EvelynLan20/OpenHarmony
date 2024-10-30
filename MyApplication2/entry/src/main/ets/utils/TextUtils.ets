/**
 * 字符串工具类
 */
import { Function0, NType } from './NfTypesUtil';
import measure from '@ohos.measure'

export class TextUtils {
  public static isEmpty(text: NType<string>) {
    if (text) {
      return text.length <= 0
    } else {
      return true
    }
  }

  public static isNotEmpty(text: NType<string>) {
    return !TextUtils.isEmpty(text)
  }

  /**
   * 返回非空
   * @param text
   * @param defaultValue 默认值 或者是一个lambda函数
   * @returns 非空
   */
  public static ifEmpty(text: NType<string>, defaultValue: string | Function0<string>): string {
    if (TextUtils.isNotEmpty(text)) {
      return text!
    }
    if (defaultValue instanceof Function) {
      return defaultValue()
    }
    return defaultValue
  }

  public static getTextWidth(text: string, fontSize: number): number {
    return px2vp(
      measure.measureTextSize({
        textContent: text,
        fontSize: fontSize,
        maxLines: 1
      }).width as number ?? 0)
  }
}