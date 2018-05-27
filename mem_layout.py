Import("env")

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "xtensa-lx106-elf-nm", "--print-size", "--size-sort", "--radix=d", "$BUILD_DIR/${PROGNAME}.elf", ">", "mem_layout.csv"
    ]), "Creating app-symbols.csv")
)
