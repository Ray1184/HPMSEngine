/*!
 * File FileSystem.cpp
 */

#include <common/FileSystem.h>

bool hpms::FileSystem::init = false;
std::vector<std::string> hpms::FileSystem::mountedPaths = std::vector<std::string>();

int hpms::FileSystem::MountFS(const std::string& pak)
{
    if (!init)
    {
        PHYSFS_init(nullptr);
        init = true;
    }
    if (std::find(mountedPaths.begin(), mountedPaths.end(), pak) == mountedPaths.end())
    {
        mountedPaths.push_back(pak);
        return PHYSFS_mount(pak.c_str(), nullptr, 1);
    } else
    {
        return 0;
    }
}

void* hpms::FileSystem::LoadRawResource(const std::string& path, size_t* size)
{
    if (PHYSFS_exists(path.c_str()))
    {
        PHYSFS_file* file = PHYSFS_openRead(path.c_str());
        int fileLength = PHYSFS_fileLength(file);
        char* buf = hpms::SafeNewArray<char>(fileLength);
        int lengthRead = PHYSFS_readBytes(file, buf, fileLength);
        *size = lengthRead;
        PHYSFS_close(file);
        return buf;
    } else
    {
        std::stringstream ss;
        ss << "Impossible to retrieve " << path << " from mounted archive.";
        LOG_ERROR(ss.str().c_str());

    }
}

template<typename T>
T* hpms::FileSystem::LoadBinResource(const std::string& path, size_t* size)
{
    T* t = hpms::SafeNew<T>();
    void* buf = LoadRawResource(path, size);
    pods::InputBuffer in((const char*) buf, *size);
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    pods::Error err = deserializer.load(*t);
    if (err != pods::Error::NoError)
    {

        switch (err)
        {
            case pods::Error::ArchiveVersionMismatch:
                LOG_ERROR("An error occurred deserializing: ArchiveVersionMismatch");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::MandatoryFieldMissed:
                LOG_ERROR("An error occurred deserializing: MandatoryFieldMissed");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::OptionalFieldMissed:
                LOG_ERROR("An error occurred deserializing: OptionalFieldMissed");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::CorruptedArchive:
                LOG_ERROR("An error occurred deserializing: CorruptedArchive");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::NotEnoughMemory:
                LOG_ERROR("An error occurred deserializing: NotEnoughMemory");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::UnexpectedEnd:
                LOG_ERROR("An error occurred deserializing: UnexpectedEnd");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::SizeToLarge:
                LOG_ERROR("An error occurred deserializing: SizeToLarge");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::WriteError:
                LOG_ERROR("An error occurred deserializing: WriteError");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::ReadError:
                LOG_ERROR("An error occurred deserializing: ReadError");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            case pods::Error::Eof:
                LOG_ERROR("An error occurred deserializing: Eof");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
            default:
                LOG_ERROR("An error occurred deserializing: UnknownError");
                hpms::SafeDeleteArray(buf);
                exit(EXIT_FAILURE);
        }
    }
    hpms::SafeDeleteArray(buf);
    return t;

}
