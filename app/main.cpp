#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>
namespace fs = std::filesystem;

int main(int argc, char **argv) {
  try {
    if (argc < 3) {
      fprintf(stderr,
              "USAGE: %s {resource} {symbol}\n\n"
              "  Creates {symbol} from the contents of {resource}\n",
              argv[0]);
      return EXIT_FAILURE;
    }
    fs::path resource_path(argv[1]);
    fs::path symbol_path(argv[2]);
    if (!fs::exists(symbol_path.parent_path())) {
      fs::create_directories(symbol_path.parent_path());
    }

    std::string symbol = symbol_path.stem().stem().string();
    std::replace(symbol.begin(), symbol.end(), '.', '_');
    if (!fs::exists(resource_path)) {
      throw std::runtime_error("Resource path \"" + std::string(argv[1]) +
                               "\" does not exists!");
    }
    std::ifstream in(argv[1], std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::binary | std::ios_base::trunc);
    if (!out) {
      throw std::runtime_error("Failed to open output file at \"" +
                               std::string(argv[2]) + "\"");
    }
    out << "#ifndef EMBEDCPP_" << symbol << "_H\n";
    out << "#define EMBEDCPP_" << symbol << "_H\n";
    out << "#include <string_view>\n";
    out << "namespace embedcpp {\n";
    out << "inline constexpr char " << symbol << "_c_arr[] =\n{";

    char buf[256];
    size_t read_count = 0;
    size_t linecount = 0;
    do {
      in.read(buf, sizeof(buf));
      for (size_t i = 0; i < in.gcount(); ++i) {
        out << "0x" << std::hex << (buf[i] & 0xff) << ", ";
        if (++linecount == 10) {
          out << "\n";
          linecount = 0;
        }
      }
    } while (in.gcount() > 0);

    out << "};\n";
    out << "inline constexpr std::string_view " << symbol << "_view(" << symbol
        << "_c_arr, std::size(" << symbol << "_c_arr));\n";
    out << "} //namespace embedcpp\n";
    out << "#endif\n";

  } catch (const std::exception &e) {
    fprintf(stderr, "Expection caught on main: %s\n", e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
