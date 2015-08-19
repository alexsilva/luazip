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
print("unzip_filepath:", unzip_filepath)
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

print("-------")
print("unzip_states:", unzip_states)
print("UNZIP_SUCCESS:", unzip_states.UNZIP_SUCCESS)
print("UNZIP_ERROR:", unzip_states.UNZIP_ERROR)
print("-------")

local samples_dir =  root_dir.."/samples"
local zip_filename = samples_dir .."/sample2.zip";

local status, zip = zip_open(zip_filename, zip_open_flags.ZIP_CREATE)

if (status ~= 0) then
    print(format("[%i] %s", status, zip))
else
    local dirname = "zipfile/samples"
    print("dir created:", zip_add_dir(zip, dirname))

    zip_add_file(zip, samples_dir .."/sample.zip", dirname.."/sample.zip")

    -- num of entries
    print("zip_get_num_entries: ", zip_get_num_entries(zip))

    -- locate file
    print("zip_name_locate: ", zip_name_locate(zip, dirname.."/sample.zip"))
    print("zip_name_locate: ", zip_name_locate(zip, "sample.zip", zip_file_encoding.ZIP_FL_NODIR))

    print("zip_close: ", zip_close(zip))
end

status, zip = zip_open(zip_filename)
local code, msg

if (status == 0) then
    code, msg = unzip(zip, samples_dir.."/extractfiles")
    print("unzip status:", code, msg)
    print("zip_close: ", zip_close(zip))
end

code, msg = unzip_filepath(samples_dir.."/sample.zip", samples_dir.."/extractfiles")
print("unzip by path status:", code, msg)