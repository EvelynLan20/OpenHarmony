import { uiObserver } from '@kit.ArkUI';


@Entry
@Component
struct NavigationPage {
  @State message: string = 'Navigation page'
  @State TooTmp: ToolbarItem = {
    'value': "func", 'icon': $r('app.media.startIcon'), 'action': () => {
    }
  }
  @Provide('pageInfos') pageInfos: NavPathStack = new NavPathStack()
  private arr: number[] = [1, 2, 3, 4];

  build() {
    Row() {
      Column() {
        Navigation(this.pageInfos) {
          TextInput({ placeholder: 'search...' })
            .width("90%")
            .height(40)
            .backgroundColor('#FFFFFF')

          List({ space: 12 }) {
            ForEach(this.arr, (item: string, index: number) => {
              ListItem() {
                if (index == 3) {
                  Text("DialogPage" + item)
                    .width("100%")
                    .height(72)
                    .backgroundColor('#FFFFFF')
                    .borderRadius(24)
                    .fontSize(16)
                    .fontWeight(500)
                    .textAlign(TextAlign.Center)
                    .onClick(() => {
                      this.pageInfos.pushPathByName('DialogPage' + item, '');
                    })
                } else {
                  Text("NavRouter" + item)
                    .width("100%")
                    .height(72)
                    .backgroundColor('#FFFFFF')
                    .borderRadius(24)
                    .fontSize(16)
                    .fontWeight(500)
                    .textAlign(TextAlign.Center)
                    .onClick(() => {
                      this.pageInfos.pushPath({ name: "NavDestinationTitle" + item, param: 'send message' })
                    })
                }
              }
            }, (item: string): string => item)
          }
          .width("90%")
          .margin({ top: 12 })
        }
        .title("主标题")
        .mode(NavigationMode.Stack)
        .navDestination(this.PageMap)
        .titleMode(NavigationTitleMode.Mini)
        .menus([
          {
            value: "1", icon: 'resources/rawfile/ic_public_add.svg', action: () => {
          }
          },
          {
            value: "2", icon: 'resources/rawfile/ic_public_search.svg', action: () => {
          }
          },
          {
            value: "3", icon: 'resources/rawfile/ic_public_add.svg', action: () => {
          }
          },
          {
            value: "4", icon: 'resources/rawfile/ic_public_add.svg', action: () => {
          }
          },
          {
            value: "5", icon: 'resources/rawfile/ic_public_add.svg', action: () => {
          }
          }
        ])
        .toolbarConfiguration([this.TooTmp, this.TooTmp, this.TooTmp])
      }
      .width('100%')
    }
    .height('100%')
  }

  @Builder
  PageMap(name: string) {
    if (name === "NavDestinationTitle1") {
      pageOneTmp()
    } else if (name === "NavDestinationTitle2") {
      pageTwoTmp()
    } else if (name === "NavDestinationTitle3") {
      pageThreeTmp()
    } else if (name === "DialogPage4") {
      DialogPage()
    }
  }
}

// PageOne.ets
@Component
export struct pageOneTmp {
  @Consume('pageInfos') pageInfos: NavPathStack;
  @State message: string = ''

  aboutToAppear(): void {
    this.message = this.pageInfos.getParamByName("NavDestinationTitle1").toString()
  }

  build() {
    NavDestination() {
      Column() {
        Text("NavDestinationContent1")
        Text(this.message)
      }.width('100%').height('100%')
    }.title("NavDestinationTitle1")
    .onBackPressed(() => {
      const popDestinationInfo = this.pageInfos.pop() // 弹出路由栈栈顶元素
      console.log('pop' + '返回值' + JSON.stringify(popDestinationInfo))
      return true
    })
  }
}

// PageTwo.ets
@Component
export struct pageTwoTmp {
  @Consume('pageInfos') pageInfos: NavPathStack;

  aboutToAppear(): void {
    this.pageInfos.disableAnimation(true)
  }

  aboutToDisappear(): void {
    this.pageInfos.disableAnimation(false)
  }

  build() {
    NavDestination() {
      Column() {
        Text("NavDestinationContent2")
      }.width('100%').height('100%')
    }.title("NavDestinationTitle2")
    .onBackPressed(() => {
      const popDestinationInfo = this.pageInfos.pop() // 弹出路由栈栈顶元素
      console.log('pop' + '返回值' + JSON.stringify(popDestinationInfo))
      return true
    })
  }
}

// PageThree.ets
@Component
export struct pageThreeTmp {
  @Consume('pageInfos') pageInfos: NavPathStack;
  @State isShow: boolean = false

  build() {
    NavDestination() {
      Column() {
        Stack({ alignContent: Alignment.Center }) {
          if (this.isShow) {
            Image($r('app.media.startIcon'))
              .autoResize(false)
              .clip(true)
              .width(300)
              .height(300)
              .offset({ y: 100 })
              .geometryTransition("picture", { follow: false })
              .transition(TransitionEffect.OPACITY)
          } else {
            // geometryTransition此处绑定的是容器，那么容器内的子组件需设为相对布局跟随父容器变化，
            // 套多层容器为了说明相对布局约束传递
            Column() {
              Column() {
                Image($r('app.media.startIcon'))
                  .width('100%').height('100%')
              }.width('100%').height('100%')
            }
            .width(80)
            .height(80)
            // geometryTransition会同步圆角，但仅限于geometryTransition绑定处，此处绑定的是容器
            // 则对容器本身有圆角同步而不会操作容器内部子组件的borderRadius
            .borderRadius(20)
            .clip(true)
            .geometryTransition("picture")
            // transition保证组件离场不被立即析构，可设置其他转场效果
            .transition(TransitionEffect.OPACITY)
          }
        }
        .onClick(() => {
          animateTo({ duration: 1000 }, () => {
            this.isShow = !this.isShow
          })
        })

        Button('共享元素转场')
          .width('80%')
          .height(40)
          .margin(20)
          .offset({ y: this.isShow ? 100 : 0 })
          .onClick(() => {
            animateTo({ duration: 1000 }, () => {
              this.isShow = !this.isShow
            })
          })
      }.width('100%').height('100%')

    }.title("NavDestinationTitle3")
    .onBackPressed(() => {
      const popDestinationInfo = this.pageInfos.pop() // 弹出路由栈栈顶元素
      console.log('pop' + '返回值' + JSON.stringify(popDestinationInfo))
      return true
    })
  }
}

@Component
export struct DialogPage {
  @Consume('pageInfos') pageStack: NavPathStack;
  @State navDesInfo: uiObserver.NavDestinationInfo | undefined = undefined
  @State pageName: ResourceStr = ""

  aboutToAppear(): void {
    this.navDesInfo = this.queryNavDestinationInfo();
    uiObserver.on('navDestinationUpdate', (info) => {
      this.pageName = info.name
      console.info('NavDestination state update', JSON.stringify(info));
    });
  }

  build() {
    NavDestination() {
      Stack({ alignContent: Alignment.Center }) {
        Column() {
          Text("所属页面Name: " + this.pageName)
            .fontColor(Color.Black)
            .fontSize(20)
            .margin({ bottom: 20 })

          Text("Dialog NavDestination")
            .fontColor(Color.Black)
            .fontSize(20)
            .margin({ bottom: 50 })
          Button("Close").onClick(() => {
            this.pageStack.pop()
          }).width('30%')
        }
        .justifyContent(FlexAlign.Center)
        .backgroundColor(Color.White)
        .borderRadius(10)
        .height('30%')
        .width('80%')
      }.height("100%").width('100%')
    }
    .backgroundColor('rgba(0,0,0,0.5)')
    .hideTitleBar(true)
    .mode(NavDestinationMode.DIALOG)
  }
}