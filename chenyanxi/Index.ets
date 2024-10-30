import { router } from '@kit.ArkUI';

@Entry
@Component
struct Index {
  // chenyanxi
  @State message: string = '电鸿7组'
  // laiaining
  // @State message: string = '9组团队任务'
  // linyukun
  // @State message: string = 'Home'


  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)

        // Button() {
        //   Text(' Router page')
        //     .fontSize(30)
        //     .fontWeight(FontWeight.Bold)
        // }
        // .type(ButtonType.Capsule)
        // .margin({
        //   top:20
        // })
        // .backgroundColor('#0D9FFB')
        // .width('80%')
        // .height('5%')
        // .onClick(() => {
        //   router.pushUrl({
        //     url: 'pages/RouterPage',
        //     params: {
        //       data1: 'send message',
        //       data2: {
        //         data3: [123, 456, 789]
        //       }
        //     }
        //   });
        // })

        // Button() {
        //   Text('Navigation page')
        //     .fontSize(30)
        //     .fontWeight(FontWeight.Bold)
        // }
        // .type(ButtonType.Capsule)
        // .margin({
        //   top: 20
        // })
        // .backgroundColor('#0D9FFB')
        // .width('80%')
        // .height('5%')
        // .onClick(() => {
        //   router.pushUrl({
        //     url: 'pages/NavigationPage'
        //   });
        // })
        //
        // Button() {
        //   Text('Mqtt page')
        //     .fontSize(30)
        //     .fontWeight(FontWeight.Bold)
        // }
        // .type(ButtonType.Capsule)
        // .margin({
        //   top: 20
        // })
        // .backgroundColor('#0D9FFB')
        // .width('80%')
        // .height('5%')
        // .onClick(() => {
        //   router.pushUrl({
        //     url: 'pages/MqttPage'
        //   });
        // })

        // chenyanxi
        Button() {
          Text('温度监测')  //
            .fontSize(30)
            .fontWeight(FontWeight.Bold)
        }
        .type(ButtonType.Capsule)
        .margin({
          top: 20
        })
        .backgroundColor('#0D9FFB')
        .width('80%')
        .height('5%')
        .onClick(() => {
          router.pushUrl({
            url: 'pages/FilePage'
          });
        })

        // Laiaining
        // Button() {
        //   Text('查看温度')  //
        //     .fontSize(30)
        //     .fontWeight(FontWeight.Bold)
        //     .fontColor("#ffffff")
        // }
        // .type(ButtonType.Capsule)
        // .margin({
        //   top: 20
        // })
        // .backgroundColor('#ff840dfb')
        // .width('80%')
        // .height('5%')
        // .onClick(() => {
        //   router.pushUrl({
        //     url: 'pages/FilePage'
        //   });
        // })


      }
      .width('100%')
    }
    .height('100%')

  }
}