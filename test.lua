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
print("zip_add_dir:", zip_add_dir)
print("zip_add_file:", zip_add_file)
print("zip_discard:", zip_discard)
print("zip_get_num_entries:", zip_get_num_entries)
print("zip_name_locate:", zip_name_locate)
print("unzip:", unzip)

print("-------")
print("zip_open_flags:", zip_open_flags)
print("ZIP_CHECKCONS: ", zip_open_flags.ZIP_CHECKCONS)
print("ZIP_CREATE: ",    zip_open_flags.ZIP_CREATE)
print("ZIP_TRUNCATE: ",  zip_open_flags.ZIP_TRUNCATE)
print("ZIP_RDONLY: ",    zip_open_flags.ZIP_RDONLY)

print("-------")
print("zip_encoding:", zip_file_encoding)
print("ZIP_FL_NOCASE:", zip_file_encoding.ZIP_FL_NOCASE)
print("ZIP_FL_NODIR:", zip_file_encoding.ZIP_FL_NODIR)
print("ZIP_FL_ENC_RAW:", zip_file_encoding.ZIP_FL_ENC_RAW)
print("ZIP_FL_ENC_GUESS:", zip_file_encoding.ZIP_FL_ENC_GUESS)
print("ZIP_FL_ENC_STRICT:", zip_file_encoding.ZIP_FL_ENC_STRICT)

local zip_filename = root_dir.."/samples/sample2.zip";

local status, zip = zip_open(zip_filename, zip_open_flags.ZIP_CREATE)

if (status ~= 0) then
    print(format("[%i] %s", status, zip))
else
    local dirname = "zipfile/samples"
    print("dir created:", zip_add_dir(zip, dirname))

    zip_add_file(zip, root_dir.."/samples/sample.zip", dirname.."/sample.zip")

    -- num of entries
    print("zip_get_num_entries: ", zip_get_num_entries(zip))

    -- locate file
    print("zip_name_locate: ", zip_name_locate(zip, dirname.."/sample.zip"))
    print("zip_name_locate: ", zip_name_locate(zip, "sample.zip", zip_file_encoding.ZIP_FL_NODIR))

    print( zip_close(zip) )
end

status, zip = zip_open(zip_filename)

if (status == 0) then
    unzip(zip, root_dir.."/samples/extractfiles")
    print( zip_close(zip) )
end
