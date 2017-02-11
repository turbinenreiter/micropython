import dbus

def foob() -> bool:
    return(True)

def fooy() -> int:
    return(64)

def foon() -> int:
    return(64)

def fooi() -> int:
    return(64)

def foox() -> int:
    return(64)

def fooq() -> int:
    return(64)

def foou() -> int:
    return(64)

def foot() -> int:
    return(64)

def food() -> float:
    return(3.1)

def foos() -> str:
    return('hello')

def fooab() -> (bool):
    return((True, False, True, False))

def fooax() -> (bool):
    return((10, 11, 12, 13))

def foobb(inp: bool) -> bool:
    return(not inp)

def fooyy(inp: int) -> int:
    return(inp+1)

def foonn(inp: int) -> int:
    return(inp+1)

def fooii(inp: int) -> int:
    return(inp+1)

def fooxx(inp: int) -> int:
    return(inp+1)

def fooqq(inp: int) -> int:
    return(inp+1)

def foouu(inp: int) -> int:
    return(inp+1)

def foott(inp: int) -> int:
    return(inp+1)

def foodd(inp: float) -> float:
    return(inp*0.5)

def fooss(inp: str) -> str:
    return('!'+inp)

def foobs(inp: bool) -> str:
    return('!'+str(inp))

def fooxd(inp: int) -> float:
    return(float(inp/2))

def foodx(inp: float) -> int:
    return(int(inp))

def foosb(inp: str) -> bool:
    return(True)

def fooad() -> (float):
    return((1.1,2.2,3.3))

def fooxad(inp: int) -> (float):
    return((inp+0.1,inp+0.2,inp+0.3))

print('registered', dbus.register(foob, 'NULL', 'b'))
print('registered', dbus.register(fooy, 'NULL', 'y'))
print('registered', dbus.register(foon, 'NULL', 'n'))
print('registered', dbus.register(fooi, 'NULL', 'i'))
print('registered', dbus.register(foox, 'NULL', 'x'))
print('registered', dbus.register(fooq, 'NULL', 'q'))
print('registered', dbus.register(foou, 'NULL', 'u'))
print('registered', dbus.register(foot, 'NULL', 't'))
print('registered', dbus.register(food, 'NULL', 'd'))
print('registered', dbus.register(foos, 'NULL', 's'))
print('registered', dbus.register(fooab, 'NULL', 'ab'))
print('registered', dbus.register(fooax, 'NULL', 'ax'))
print('registered', dbus.register(fooad, 'NULL', 'ad'))
print('registered', dbus.register(foobb, 'b', 'b'))
print('registered', dbus.register(fooyy, 'y', 'y'))
print('registered', dbus.register(foonn, 'n', 'n'))
print('registered', dbus.register(fooii, 'i', 'i'))
print('registered', dbus.register(fooxx, 'x', 'x'))
print('registered', dbus.register(fooqq, 'q', 'q'))
print('registered', dbus.register(foouu, 'u', 'u'))
print('registered', dbus.register(foott, 't', 't'))
print('registered', dbus.register(foodd, 'd', 'd'))
print('registered', dbus.register(fooss, 's', 's'))
print('registered', dbus.register(foobs, 'b', 's'))
print('registered', dbus.register(fooxd, 'x', 'd'))
print('registered', dbus.register(foodx, 'd', 'x'))
print('registered', dbus.register(foosb, 's', 'b'))
print('registered', dbus.register(fooxad, 'x', 'ad'))

print('init space.plamauer.test', dbus.init('space.plamauer.test', '/space/plamauer/test'))

print('run space.plamauer.test')
for i in range(10):
    print('process requests', dbus.process(1))
    print('signal', dbus.signal('space.plamauer.signal', '/space/plamauer/signal', 'test_signal'))

print('deinit space.plamauer.test', dbus.deinit())
