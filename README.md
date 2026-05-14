
# INV-MCU_Lab

> **Gasoline Generator Digital Inverter Development Kit**  
> **汽油发电机数字逆变器开发套件**

INV-MCU_Lab 是一个面向工程学习、技术研究、项目复盘和方案验证的汽油发电机数字逆变器开源开发套件。

本项目围绕一套真实的汽油发电机数字逆变器软硬件系统展开，内容覆盖 MCU 控制软件、硬件参考资料、技术文档和测试报告，重点展示 MCU 在汽油发电机数字逆变器中的控制、采样、保护、调试和系统协同作用。

> **重要说明**  
> 虽然本项目名称中使用“开发套件”，但它不是一个建议直接量产的商业成品方案，也不是一个面向零基础用户的直接上电 DIY 套件。  
> 本项目涉及高压直流母线、230V 交流输出、大功率开关器件、汽油发动机系统和复杂保护逻辑。任何硬件复刻、上电测试或商业使用都必须由具备相关经验的工程人员完成完整验证。

---

## 1. Project Name / 项目名称

```text
English Name: INV-MCU_Lab
Chinese Name: 汽油发电机数字逆变器开发套件
```

项目名称含义：

- `INV`：Inverter，逆变器；
- `MCU`：Microcontroller Unit，微控制器；
- `Lab`：Laboratory，实验室 / 开发实验平台。

因此，INV-MCU_Lab 的核心定位是：

```text
一个用于学习、研究和理解汽油发电机数字逆变器 MCU 控制系统的开源开发套件。
```

---

## 2. Project Positioning / 项目定位

INV-MCU_Lab 不是一个简单的代码仓库，而是一个围绕真实工程项目整理出来的数字逆变器开发参考包。

它的重点不是鼓励用户直接复制一台可以量产的逆变器，而是帮助工程师理解以下问题：

- MCU 如何参与汽油发电机逆变器控制；
- PWM 如何通过查表、调制和占空比更新变成 SPWM；
- ADC 采样值如何换算成真实电压、电流和母线状态；
- 数字逆变器为什么需要软启动；
- 过压、欠压、过流、过载、过温等保护逻辑如何组织；
- 发电机运行状态、母线电压、逆变输出和 MCU 控制之间如何配合；
- MCU 软件如何与驱动电路、功率器件、采样电路和保护电路协同工作；
- 工程调试中如何通过串口输出、变量观测和故障码定位问题。

本项目更适合作为：

```text
工程学习资料
数字逆变器控制参考
MCU 控制软件架构样例
汽油发电机逆变器项目复盘资料
高压功率电子项目的系统级学习案例
```

---

## 3. Open Source Scope / 开源范围

本项目计划开源范围为：

```text
C. 软件 + 硬件 + 文档 + 测试报告
```

具体包括以下几类内容。

### 3.1 Firmware / 软件源码

软件部分主要包括：

- BAT32G139 MCU 工程源码；
- 系统初始化代码；
- PWM / SPWM 输出逻辑；
- ADC 采样与数据处理；
- 母线电压采样与补偿；
- 输出电压 / 输出电流采样处理；
- 软启动控制逻辑；
- 逆变器运行状态机；
- 保护状态机；
- 故障标志管理；
- 过压、欠压、过流、过载、短路、过温等保护逻辑；
- 串口调试输出；
- 工程参数配置文件；
- 与硬件平台相关的 BSP / Driver / App 层代码。

### 3.2 Hardware / 硬件资料

硬件部分根据整理情况可能包括：

- 原理图；
- PCB 文件；
- PCB 参考图；
- BOM；
- Gerber；
- 关键功率器件说明；
- 采样电路说明；
- 驱动电路说明；
- 辅助电源说明；
- 保护电路说明；
- 硬件接口定义。

### 3.3 Documentation / 技术文档

文档部分可能包括：

- 系统框图；
- 软件架构说明；
- 控制流程说明；
- SPWM 生成说明；
- ADC 采样换算说明；
- 软启动说明；
- 保护状态机说明；
- 故障码说明；
- 串口调试协议说明；
- 调试记录；
- 项目复盘笔记；
- 已知问题说明。

### 3.4 Test Report / 测试报告

测试报告部分可能包括：

- 型式试验报告；
- 调试测试记录；
- 关键波形记录；
- 负载测试记录；
- 故障保护测试记录；
- 工程验证记录。

由于测试报告、PDF、压缩包等文件可能较大，不建议全部直接提交到 Git 仓库主分支。大文件建议通过 GitHub Releases 发布。

---

## 4. What This Project Is / 本项目是什么

INV-MCU_Lab 是：

```text
一个汽油发电机数字逆变器 MCU 控制开发套件；
一个真实工程项目的开源学习版本；
一个用于理解数字逆变器软硬件协同的参考项目；
一个包含软件、硬件、文档和测试资料的系统级学习案例。
```

本项目可以帮助使用者学习：

- 汽油发电机逆变器的基本系统构成；
- MCU 在逆变器中的作用；
- SPWM 与功率桥之间的关系；
- ADC 采样与实际电气量之间的换算；
- 软件保护逻辑如何与硬件保护配合；
- 故障标志如何设计；
- 状态机如何组织；
- 串口调试如何服务于真实项目排障；
- 软件、硬件、功率器件和测试方法之间的关系。

---

## 5. What This Project Is Not / 本项目不是什么

为了避免误解，INV-MCU_Lab 明确不是以下内容。

### 5.1 不是直接量产方案

本项目虽然包含软件、硬件、文档和测试报告，但开源版本仍然仅作为学习、研究和工程参考使用。

不建议未经完整验证、安规评估、EMC 测试、可靠性测试、长时间负载测试和生产一致性验证，直接用于商业量产。

### 5.2 不是零基础 DIY 套件

本项目涉及高压、大功率和汽油发电机系统，不适合没有电力电子经验、没有隔离测试设备、没有示波器和差分探头经验的用户直接复刻或上电。

### 5.3 不是安全认证过的产品

本项目本身不代表已经通过任何国家或地区的安全认证、EMC 认证、能效认证、环保认证或产品合规认证。

### 5.4 不是商业技术支持服务

本项目以开源资料形式提供。作者不承诺提供商业级技术支持、定制开发、远程调试、量产问题兜底或售后服务。

### 5.5 不是第三方软件 / 工具的再分发仓库

本项目不会提供商业软件安装包、破解工具、注册机、授权文件或未经许可的第三方资料。

如果项目需要 Keil、芯片 Pack、串口工具、VOFA+、烧录工具等，请使用者自行从官方渠道获取。

---

## 6. Commercial Use Boundary / 商用边界

本项目允许用于：

```text
学习
研究
技术交流
工程参考
个人实验
非商业项目验证
```

本项目不建议直接用于：

```text
商业量产
客户项目直接交付
未验证的整机产品
面向普通用户销售的成品设备
绕过安全认证的商业应用
```

如果你希望将本项目用于商业产品、批量生产或客户项目，请务必自行完成以下工作：

- 完整硬件设计复核；
- 软件代码审查；
- 控制参数重新标定；
- 采样系数重新校准；
- 保护阈值重新确认；
- 功率器件选型验证；
- 热设计验证；
- 安规设计；
- EMC 测试；
- 高低温测试；
- 老化测试；
- 满载和过载测试；
- 短路和故障注入测试；
- 发动机实际工况测试；
- 生产测试流程设计；
- 不同国家和地区的法规合规评估。

作者不对任何直接或间接商业使用结果承担责任。

---

## 7. Technical Scope / 技术范围边界

本项目重点关注以下内容：

```text
MCU 控制逻辑
数字逆变器软件架构
SPWM 生成
ADC 采样与换算
母线电压处理
软启动控制
保护状态机
故障标志管理
串口调试
汽油发电机逆变器系统级理解
```

本项目不重点解决以下问题：

```text
最终量产设计
批量生产工艺
安规认证
EMC 认证
整机结构设计
散热器批量制造
生产测试工装
商业售后体系
不同地区法规认证
直接可销售成品设计
```

---

## 8. Hardware Safety Boundary / 硬件安全边界

本项目涉及的硬件系统可能包含：

- 高压直流母线；
- 230V AC 逆变输出；
- 大功率 MOSFET / IGBT / 驱动电路；
- 电压采样和电流采样电路；
- 汽油发动机发电绕组；
- 辅助电源；
- 继电器、风扇、保护电路；
- 散热器和高温功率器件。

错误操作可能导致：

- 触电；
- 功率器件炸裂；
- PCB 烧毁；
- 火灾；
- 发电机损坏；
- 测试仪器损坏；
- 人身伤害。

使用者在任何上电测试前，必须自行准备并确认：

- 隔离电源；
- 差分探头；
- 示波器安全接法；
- 限流措施；
- 保险丝或断路保护；
- 电子负载或合适负载；
- 绝缘防护；
- 紧急断电措施；
- 高压测试安全区域；
- 足够的电力电子和嵌入式调试经验。

---

## 9. Suggested Audience / 适合人群

本项目适合：

- 嵌入式工程师；
- 电力电子工程师；
- 汽油发电机逆变器研发人员；
- 想学习 SPWM、ADC 采样、保护状态机的工程人员；
- 想通过真实项目理解 MCU 如何控制功率硬件的学习者；
- 对数字逆变器控制软件架构感兴趣的人；
- 希望研究汽油发电机逆变器系统构成的人。

本项目不适合：

- 完全没有高压电路经验的初学者；
- 只想找一个直接复制量产方案的人；
- 没有示波器、隔离设备、负载设备的用户；
- 不理解高压大功率风险的用户；
- 希望绕过工程验证直接销售产品的人。

---

## 10. Main Features / 主要功能模块

当前项目重点覆盖以下功能模块：

- MCU system initialization；
- PWM / SPWM generation；
- ADC sampling；
- DC bus voltage processing；
- AC output voltage processing；
- AC output current processing；
- soft-start control；
- inverter state machine；
- protection state machine；
- fault flag management；
- overload protection logic；
- short-circuit protection logic；
- over-voltage and under-voltage protection logic；
- UART debug output；
- engineering parameter configuration；
- generator inverter system analysis notes。

---

## 11. Repository Structure / 仓库目录结构

建议仓库结构如下：

```text
INV-MCU_Lab/
│
├─ firmware/
│  └─ INV-MCU_Lab/
│     ├─ App/
│     ├─ BSP/
│     ├─ Driver/
│     ├─ User/
│     └─ README.md
│
├─ hardware/
│  ├─ schematic/
│  ├─ pcb/
│  ├─ bom/
│  ├─ gerber/
│  └─ README.md
│
├─ docs/
│  ├─ architecture/
│  ├─ debug_notes/
│  ├─ test_report/
│  └─ README.md
│
├─ tools/
│  ├─ scripts/
│  └─ README.md
│
├─ release_assets/
│  └─ local_only_not_committed/
│
├─ README.md
├─ LICENSE
├─ NOTICE.md
├─ DISCLAIMER.md
└─ .gitignore
```

目录说明：

- `firmware/`：MCU 源码、Keil 工程、驱动层和应用层代码；
- `hardware/`：原理图、PCB、BOM、Gerber 和硬件说明；
- `docs/`：系统说明、调试记录、框图、测试报告说明和技术文档；
- `tools/`：工具使用说明和可公开脚本；
- `release_assets/`：本地临时目录，不提交到 Git 仓库，用于整理大 PDF、压缩包和发布附件。

---

## 12. Release Downloads / 发布附件下载

```
Large files, tool packages and release assets are available from GitHub Releases.

大文件、工具包和发布附件请从 GitHub Releases 页面下载。

Release page:

```text
https://github.com/leekh0314/INV-MCU_Lab/releases

---

## 13. Development Environment / 开发环境

当前项目可能涉及以下开发环境：

```text
MCU: BAT32G139
IDE: Keil MDK
Language: C
Application: 3KW / 230V gasoline generator digital inverter
```

具体编译器版本、芯片 Pack、工程配置和烧录说明，应在 `firmware/BAT32G139/README.md` 中进一步说明。

---

## 14. Build and Debug / 编译与调试

建议后续在固件目录中补充以下内容：

- Keil 工程打开方法；
- 编译器版本要求；
- 芯片 Pack 安装说明；
- 下载器 / 烧录器说明；
- 串口调试波特率；
- 串口调试协议；
- 调试变量说明；
- 常见编译错误；
- 常见硬件调试注意事项；
- 上电前检查清单。

---

## 15. Documentation Plan / 文档计划

后续文档可以按以下主题逐步补充：

```text
01_system_overview.md
02_pwm_spwm.md
03_adc_sampling.md
04_soft_start.md
05_protection_state_machine.md
06_uart_debug.md
07_fault_code.md
08_hardware_overview.md
09_test_report_notes.md
10_known_issues.md
```

---

## 16. Known Risks / 已知风险

本项目至少存在以下风险：

- 不同硬件版本之间参数可能不完全一致；
- 不同功率器件、采样电阻、母线电压和驱动电路会影响控制参数；
- 软件保护阈值必须结合实际硬件重新校准；
- ADC 换算系数需要根据实际采样电路确认；
- PWM 死区、驱动逻辑和功率器件参数必须严格匹配；
- 直接照搬源码可能导致输出异常、器件损坏或安全事故；
- 文档和代码可能存在未完全同步的地方；
- 测试报告中的结论不能自动等同于所有复刻硬件都能达到相同结果；
- 本项目不能替代完整的产品认证和量产验证流程。

---

## 17. Contribution / 贡献方式

欢迎提交以下类型的贡献：

- 文档修正；
- 注释补充；
- 编译问题修复；
- 模块化改进；
- 调试记录补充；
- 框图和流程图补充；
- 安全警告补充；
- issue 反馈；
- 代码可读性改进。

不建议提交以下内容：

- 商业软件安装包；
- 破解工具；
- 未授权数据手册；
- 与项目无关的大文件；
- 未经验证的危险硬件修改；
- 删除安全保护逻辑的修改；
- 绕过保护机制的修改；
- 不说明风险的参数改动。

---

## 18. License / 开源协议

软件源码：MIT License
硬件资料：CERN-OHL-P
文档和测试报告：CC BY 4.0



---

## 19. Disclaimer / 免责声明

This project is provided for learning, research and engineering reference only.

本项目仅用于学习、研究和工程参考。

The author does not guarantee that this project is suitable for direct production, commercial use or direct hardware replication.

作者不保证本项目适合直接量产、商业使用或直接硬件复刻。

This project involves high voltage, high current, power inverter hardware and gasoline generator systems. Incorrect use may cause electric shock, fire, equipment damage or personal injury.

本项目涉及高压、大电流、功率逆变硬件和汽油发电机系统。错误使用可能导致触电、火灾、设备损坏或人身伤害。

Any use of this project is at your own risk.

使用本项目所产生的一切风险由使用者自行承担。

---

## 20. Recommended First Reading Order / 推荐阅读顺序

如果你是第一次接触本项目，建议按以下顺序阅读：

```text
1. README.md
2. DISCLAIMER.md
3. docs/architecture/system_overview.md
4. firmware/BAT32G139/README.md
5. docs/debug_notes/
6. hardware/README.md
7. docs/test_report/
```

---

## 21. Project Status / 项目状态

当前项目处于开源整理阶段。

后续将逐步补充：

- 更清晰的代码结构说明；
- 系统框图；
- SPWM 控制说明；
- ADC 采样说明；
- 保护状态机说明；
- 串口调试协议说明；
- 硬件资料整理；
- 测试报告和调试记录；
- Release 附件说明；
- 上电测试安全清单。

---

## 22. Final Note / 最后说明

INV-MCU_Lab 的核心价值不是鼓励用户直接复制一台逆变器，而是通过一套接近真实工程的软硬件资料，帮助工程师理解：

```text
MCU 如何参与汽油发电机数字逆变器的控制、采样、保护、调试和系统协同。
```

如果你只是想学习数字逆变器的 MCU 控制逻辑，本项目可以作为参考。

如果你想基于本项目制作真实硬件或商业产品，请务必重新完成完整的工程验证、安全评估和合规测试。

