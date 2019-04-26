#include "config.hpp"
#include "http_service.hpp"
#include "demo_server.hpp"
#include "logo.hpp"

#include <getopt.h>

using namespace Demo;

// must init logger first
INITIALIZE_EASYLOGGINGPP

static const Logger::LoggerInitializer loggerInitializer;

void print_help() {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr,
            "\t-h / --help\n\t\tshow help info\n\n"
            "\t-f / --config\n\t\tconfig file\n\n"
    );
}

bool parse_args(int argc, char *argv[]) {
    int opt;
    const char *optstr = "hf:";
    struct option long_opts[] = {
            { "help",   no_argument, nullptr, 'h' },
            { "config", no_argument, nullptr, 'f' },
            { nullptr,  no_argument, nullptr, 0 }
    };

    while ((opt = getopt_long(argc, argv, optstr, long_opts, nullptr)) != -1) {
        switch (opt) {
            case 'f':
                if (!Config::getInstance()->loadConfigFromFile(optarg)) {
                    exit(-1);
                }
                break;
            default: // case 'h':
                return false;
        }
    }

    return true;
}

void sign_handler(int sig) {
    static bool stopping = false;
    switch (sig) {
        case SIGTERM:
        case SIGINT:
            if (!stopping) {
                LOG(WARNING) << "recv a SIGINT/SIGTERM signal, exit now";
                HttpService::getInstance()->stop();
                DemoServer::getInstance()->stop();
                stopping = true;
            }
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    if (!parse_args(argc, argv)) {
        print_help();
        return EXIT_FAILURE;
    }
    Config *config = Config::getInstance();
    Logger::setCurrLogLevel(config->log_level);
    Logger::setLoggingThreadName("main_io");

    if (config->daemon_mode) {
#ifdef __linux__
        daemon(1, 0);
#endif
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGILL, SIG_IGN);
    signal(SIGINT, sign_handler);
    signal(SIGTERM, sign_handler);

    LOG(WARNING) << demo_logo;

    HttpService::getInstance()->start(config->http_listen_addr);
    // block here
    DemoServer::getInstance()->start();

    return EXIT_SUCCESS;
}
