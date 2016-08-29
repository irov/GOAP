print("Hello")

local s = Source:new()

local TaskPrint = Task("Print")

function TaskPrint:onInitialie(msg)
    self.msg = msg
end

function TaskPrint:onRun()
    print ("TaskPrint:onRun "..self.msg)
    return true
end

local TaskPrint2 = Task("Print2")

function TaskPrint2:onInitialie(msg)
    self.msg = msg
end

function TaskPrint2:onRun()
    print ("TaskPrint2:onRun "..self.msg)
    return true
end

local t0 = TaskPrint:new()
t0:onInitialie("abc0")

local t1 = TaskPrint2:new()
t1:onInitialie("abc1")

--sp = s:addParallel(2)
s0, s1 = s:addParallel(2)

s0:addTask(t0)
s1:addTask(t1)

sr1, sr2, sr3 = s:addRace(3)

local t30 = TaskPrint:new()
t30:onInitialie("abc30")

local t31 = TaskPrint:new()
t31:onInitialie("abc31")

local t32 = TaskPrint:new()
t32:onInitialie("abc32")

sr1:addTask(t30)
sr2:addTask(t31)
sr3:addTask(t32)

print("CHAIN")

local c = Chain:new(s)

print("RUN")
c:run()
