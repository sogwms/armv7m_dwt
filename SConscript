from building import *
Import('rtconfig')

cwd     = GetCurrentDir()
src	= Glob('*.c')
path = [cwd]

group = DefineGroup('dwt', src, depend = ['PKG_USING_ARMv7M_DWT'], CPPPATH = path)

Return('group')
