--
-- Created by IntelliJ IDEA.
-- User: alex
-- Date: 18/08/2015
-- Time: 17:12
-- To change this template use File | Settings | File Templates.
--
local root_dir = getenv("ROOT_DIR")
handle, msg = loadlib(getenv("LIBRARY_PATH"))

if (not handle or handle == -1) then
    error(msg)
end

callfromlib(handle, 'lua_lzipopen')

print("--- C FN ---")
print("zip_open:", zip_open)
print("zip_close:", zip_close)
print("zip_dir_add:", zip_dir_add)
print("zip_add_file:", zip_add_file)
print("zip_discard:", zip_discard)

print("-- ATTRS ---")
print("zip_open_flags:", zip_open_flags)
print("ZIP_CHECKCONS: ", zip_open_flags.ZIP_CHECKCONS)
print("ZIP_CREATE: ",    zip_open_flags.ZIP_CREATE)
print("ZIP_TRUNCATE: ",  zip_open_flags.ZIP_TRUNCATE)
print("ZIP_RDONLY: ",    zip_open_flags.ZIP_RDONLY)

status, zip = zip_open(root_dir.."/samples/sample2.zip", zip_open_flags.ZIP_CREATE)

if (status ~= 0) then
    print(format("[%i] %s", status, zip))
else
    local dirname = "samples"
    print("Add dir:", zip_add_dir(zip, dirname))
    zip_add_file(zip, root_dir.."/samples/sample.zip", dirname.."/sample.zip")
end


print( zip_close(zip) )