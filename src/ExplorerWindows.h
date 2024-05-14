#ifndef EXPLORERWINDOWS_H
#define EXPLORERWINDOWS_H

#include <filesystem>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

/**
 * @class ExplorerWindows
 * @brief Класс для управления файловой системой и подключенными устройствами в Windows.
 */
class ExplorerWindows
{
public:
    /**
     * @brief Конструктор класса ExplorerWindows.
     * Инициализирует текущий путь текущим рабочим каталогом.
     */
    ExplorerWindows();

    /**
     * @brief Отрисовывает файловую систему в окне ImGui.
     * @param font Шрифт для текста.
     * @param font32 Большой шрифт для текста.
     */
    void drawFilesystem(ImFont* font, ImFont* font32);

    /**
     * @brief Обновляет список логических дисков и подключенных устройств.
     */
    void updateDrivesAndDevices();

    /**
     * @brief Проверяет, подключено ли Android-устройство.
     * @return true, если Android-устройство подключено, иначе false.
     */
    bool isDeviceConnected();

    /**
     * @brief Получает информацию о памяти устройства.
     * @return Пара (freeMem, totalMem), где freeMem - доступная память, а totalMem - общая память в гигабайтах.
     */
    std::pair<float, float> getMemoryInfo();

private:
    fs::path currentPath; ///< Текущий путь в файловой системе.
    std::vector<std::string> pathTmp; ///< Временный путь для построения текущего пути.
    std::set<std::string> fileExt; ///< Набор расширений файлов для отображения.
    std::vector<std::string> logicalDrives; ///< Список логических дисков.
    std::vector<std::string> adbDevices; ///< Список подключенных Android-устройств.

    /**
     * @brief Открывает изображение по указанному пути.
     * @param path Путь к изображению.
     */
    void openImage(const fs::path& path);

    /**
     * @brief Выполняет команду в командной строке и возвращает результат.
     * @param cmd Команда для выполнения.
     * @return Результат выполнения команды.
     */
    std::string execCommand(const std::string& cmd);

    /**
     * @brief Получает список подключенных Android-устройств.
     * @return Вектор строк, содержащий идентификаторы устройств.
     */
    std::vector<std::string> getAdbDevices();

    /**
     * @brief Получает список файлов и папок в директории на Android-устройстве.
     * @param path Путь к директории на Android-устройстве.
     * @return Вектор строк, содержащий имена файлов и папок.
     */
    std::vector<std::string> getAndroidDirectoryEntries(const std::string& path);
};

#endif // EXPLORERWINDOWS_H
