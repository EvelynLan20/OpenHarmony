import fileio from '@ohos.fileio';
import { BusinessError } from '@kit.BasicServicesKit';
import fs from '@ohos.file.fs';
import { buffer } from '@kit.ArkTS';

@Entry
@Component
struct FilePage {
  // chenyanxi
  // @State message: string = '当前温度状态'
  // laiaining
  @State message: string = ' '
  // linyukun
  // @State message: string = 'TEMPERATURE'
  @State tempString: string = ''
  @State statusString: string = ''
  @State statusColor: string = '#FFFFFF'
  // @State debugString: string = ''

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)

        Text(this.tempString)
          .fontSize(30)
          .margin(20)
          .fontWeight(FontWeight.Bold)
        // Text(this.debugString)
        //   .fontSize(30)
        //   .margin(20)
        //   .fontWeight(FontWeight.Bold)

        Text(this.statusString)
          .fontSize(30)
          .margin(20)
          .fontWeight(FontWeight.Bold)
          .fontColor("#000000")    // 设置文字颜色
          .backgroundColor(this.statusColor)  // 设置背景颜色
          .borderRadius(20)             // 设置圆角半径为 10 像素
          .padding("10")
          .width("auto")                // 自动宽度适应文字
          .height("auto")               // 自动高度适应内容

        Button() {
          Text('温度监测')  // lai：温度监测   chen：开始获取温度   lin：start
            .fontSize(30)
            .fontWeight(FontWeight.Bold)
            .fontColor("#ffffff")
        }
        .type(ButtonType.Capsule)
        .margin({
          top: 20
        })
        .backgroundColor('#ff840dfb')
        .width('auto')
        .height('auto')
        .padding("10")
        .onClick(() => {
          // 每0.1秒读取温度
          setInterval(() => {
            const applicationContext = getContext().getApplicationContext()
            console.info('applicationContext.cacheDir====>' + applicationContext.cacheDir)
            console.info('applicationContext.filesDir====>' + applicationContext.filesDir)
            let filePath = applicationContext.cacheDir + '/output.txt'

            fs.access(filePath).then((res: boolean) => {
              if (res) {
                let file = fs.openSync(filePath, fs.OpenMode.READ_ONLY);
                let arrayBuffer = new ArrayBuffer(4096);
                fs.read(file.fd, arrayBuffer).then((readLen: number) => {
                  console.info("read file data succeed");
                  let buf = buffer.from(arrayBuffer, 0, readLen);
                  console.info(`The content of file: ${buf.toString()}`);

                  // 显示和更新内容
                  let result = buf.toString().split(" ");
                  // chenyanxi laining
                  this.tempString = "当前温度："+result[0].toString()+"°C"
                  // linyukun
                  // this.tempString = result[0].toString()+"°C"

                  let statusInt = parseInt(result[1], 10)
                  // chenyanxi
                  // this.statusString = (statusInt === 1) ? "温度过高" : "温度正常";
                  // laiaining
                  this.statusString = (statusInt === 1) ? "温度超过阈值" : "温度正常";
                  // linyukun
                  // this.statusString = (statusInt === 1) ? "温度异常！！" : "温度正常";

                  //chenyanxi laining
                  this.statusColor = (statusInt === 1) ? "#ffe20808" : "#ff01b726";
                  // linyukun
                  // this.statusColor = (statusInt === 1) ? "#ffe20808" : "#ffffffff";

                }).catch((err: BusinessError) => {
                  console.error("read file data failed with error message: " + err.message + ", error code: " + err.code);
                }).finally(() => {
                  fs.closeSync(file);
                });
              } else {
                this.tempString = 'file not exists'
                this.statusString = 'file not exists'
              }
            }).catch((err: BusinessError) => {
              console.error("access failed with error message: " + err.message + ", error code: " + err.code);
            });
          }, 100)

        })
      }
      .width('100%')
    }
    .height('100%')
  }
}

