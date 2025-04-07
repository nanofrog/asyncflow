import sys
import time
import logging
import json
sys.path.append("../flowchart")
sys.path.append("../flowchart/generate")
sys.path.append("./")
import asyncflow

class custom_logger:
	def info(self, msg):
		print('----------------', msg)
		
	def warning(self, msg):
		print('waring', msg)

	def error(self, msg):
		print('error', msg)

	def fatal(self, msg):
		print('fatal', msg)

asyncflow.set_logger(custom_logger())

asyncflow.config_log('''
[[sink]]
name = "rotate_out"
type = "rotating_file_sink_st"
base_filename = "d:/asyncflow.log"
max_size = "1M"
max_files = 10
level = "info"
                     
[[logger]]
name = "asyncflow"
sinks = ["rotate_out"]
level = "info"
''', 'asyncflow'
)


asyncflow.setup({"debug_ip":u"127.0.0.1"})

import asyncflow_events
import all_flowcharts

for item in all_flowcharts.file_list:
	root = item[0]
	file_path = item[1]
	py_file = '../flowchart/generate/' + file_path + '.py'
	exec(open(py_file).read())

print(asyncflow.EventId.Tick)

def test():
    return a + b

class Character:
    allCharacter = []
    
    def __init__(self):
        self.output = []

    def Say(self, s):
        print(s)
        raise Exception("haha")
        #test()
        #self.output.append(str(s))

    def CreateCharacter(self):
        char = Character()
        Character.allCharacter.append(char)
        char.output = self.output
        asyncflow.register(char)
        asyncflow.start(char)
        return char

def test_hotfix():
    chart_name = "AI.test_03"

    # start
    c = Character()
    asyncflow.register(c)
    asyncflow.attach(c, chart_name)
    asyncflow.start(c)

    # loop
    for i in range(40):
        time.sleep(0.1)
        asyncflow.step(100)
    # destroy
    asyncflow.remove(c, chart_name)

    asyncflow.attach(c, chart_name)
    asyncflow.start(c)

    for i in range(40):
        time.sleep(0.1)
        asyncflow.step(100)

    
    asyncflow.deregister(c)
    asyncflow.step(10)
    asyncflow.exit()

def test():
    chart_name = "AI.test_01"

    for i in range(100):
        #time.sleep(0.1)
        asyncflow.step(100)

    # start
    c = Character()
    asyncflow.register(c)
    asyncflow.attach(c, chart_name)
    asyncflow.start(c)

    # loop
    for i in range(40000000):
        time.sleep(0.1)
        asyncflow.step(100)
        if i % 30 == 0:
	        for j in range(5):
		        asyncflow.event(c, asyncflow.EventId.CustomEvent)
        #if i%10 == 0:
	    #    print(json.dumps(asyncflow.dump(c), indent=4))

    # loop
    for i in range(1):
        time.sleep(0.1)
        asyncflow.step(100)

    # destroy
    asyncflow.remove(c, chart_name)
    asyncflow.deregister(c)
    asyncflow.step(10)
    asyncflow.exit()


if __name__ == "__main__":
	test_hotfix()