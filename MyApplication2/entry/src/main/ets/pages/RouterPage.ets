import { router } from '@kit.ArkUI';

@Entry
@Component
struct RouterPage {
  @State message: string = 'router page'
  @State paramsString: string = ''

  aboutToAppear(): void {
    const params = router.getParams() as Record<string, Object>; // 获取传递过来的参数对象
    const data2 = params['data2'] as Record<string, Object>
    const data3 = data2['data3'] as Array<number>
    const data30 = data3[0].toString() + data3[1].toString() + data3[2].toString()
    this.paramsString = params['data1'].toString() + '\n' +data30
  }

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)

        Text(this.paramsString)
          .fontSize(30)
          .margin(20)
          .fontWeight(FontWeight.Bold)

        Button() {
          Text('back')
            .fontSize(30)
            .fontWeight(FontWeight.Bold)
        }
        .type(ButtonType.Capsule)
        .margin({
          top: 20
        })
        .backgroundColor('#0D9FFB')
        .width('40%')
        .height('5%')
        .onClick(() => {
          router.back()
        })
      }
      .width('100%')
    }
    .height('100%')
  }
}