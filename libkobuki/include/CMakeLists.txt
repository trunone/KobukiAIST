set(hdrs Thread.h Timer.h TimeSpec.h libkobuki.h Kobuki.h
    )

set(install_hdrs libkobuki.h Kobuki.h kobukicwrapper.h)

install(FILES ${install_hdrs} DESTINATION ${INC_INSTALL_DIR}
    COMPONENT library)

