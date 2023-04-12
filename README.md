Pre-requisite:
    Windows
        - Install GNU ARM Embedded Toolchain (required by at91bootstrap)

Initial Steps:
- Clone at91bootstrap
- In MPLABX, open "at91bootstrap" as project.
- Open Project Properties and go to KConfig.
- Load "my_harmony_driver/at9bootstrap_sama7g54_config" and click "Apply"
- Run "Clean and Build"
- In Harmony V3 Project, open Project Properties > Select "JLINK" in Hardware Tool then click "Apply".
- "Bootstrap" should show under Categories in Project Properties Box.
- Load "<\dir\>at91bootstrap\build\binaries\at91bootstrap.elf"
- "Clean and Build" Project and then "Program"