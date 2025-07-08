-- satus.lua
-- A dwmstatus replacement script.

del = ' | ' -- delimeter
if arg[1] ~= nil then del = arg[1] end

function time(format)
   if format == "" then return os.date() end
   return os.date(format)
end

function battery()
   -- returns a table with information on:
   --    current charge   (charge_now)
   --    maximum charge   (charge_max)
   --    is charging?     (charging)
   --    % charged        (pcharge)
   local bat_info = { }

   charge_now = io.input('/sys/class/power_supply/BAT0/charge_now')
   bat_info.charge_now = tonumber(io.read())
   io.close(charge_now)
   
   charge_max = io.input('/sys/class/power_supply/BAT0/charge_full')
   bat_info.charge_max = tonumber(io.read())
   io.close(charge_max)
   
   bat_status = io.input('/sys/class/power_supply/BAT0/status')
   status = io.read()
   if status == "Not charging" then bat_info.charging = false
   elseif status == "Charging" then bat_info.charging = true
   end

   bat_info.pcharge = bat_info.charge_now / bat_info.charge_max * 100

   return bat_info
end

function uptime() -- returns uptime in minutes
   local proc_file = io.input('/proc/uptime')
   t = io.read() -- TODO: implement error handling
   _, _, uptime_sec = string.find(t, '(%d+)')
   uptime_sec = nil
   if not uptime_sec then return -1 end
   io.close(proc_file)
   return uptime_sec // 60
end

function memory()
   -- returns a table with information on:
   --    total memory     (total)
   --    free memory      (free)
   --    available memory (avail)
   --    percentage free  (pfree)
   --    percentage used  (pused)
   local mem_info  = { }
   local proc_file = io.input('/proc/meminfo') -- TODO: implement error handling

   t = io.read() -- TODO: implement error handling
   _, _, mem_info.total = string.find(t, ':%s*(%d+)')
   if not mem_info.total then
      print("proc error: unable to parse meminfo.")
      return
   end

   t = io.read()
   _, _, mem_info.free = string.find(t, ':%s*(%d+)')
   if not mem_info.free then
      print("proc error: unable to parse meminfo.")
      return
   end

   t = io.read()
   _, _, mem_info.avail = string.find(t, ':%s*(%d+)')
   if not mem_info.avail then
      print("proc error: unable to parse meminfo.")
      return
   end

   io.close(proc_file)

   mem_info.pfree = mem_info.free / mem_info.avail * 100
   mem_info.pused = 100 - mem_info.pfree
   return mem_info
end

local bar = {
   string.format('RAM %.2f%%', memory().pused),
   string.format('BAT %.2f%%', battery().pcharge),
   time('%a %d %Y - %b %I:%M%p')
}

out = ''
for _, val in ipairs(bar) do
   out = out .. del .. val
end

print (out)
