
int main() {
    fd_set read_set, write_set, exception_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exception_set);

    int fd = STDIN_FILENO; // Use standard input as an example file descriptor
    FD_SET(fd, &read_set); // Add the file descriptor to the read set

    int timeout_sec = 5; // Wait up to 5 seconds for I/O events
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;

    int ready_fds = select(fd + 1, &read_set, &write_set, &exception_set, &timeout);
    if (ready_fds < 0) {
        std::cerr << "Error in select function\n";
        return 1;
    }

    if (ready_fds == 0) {
        std::cout << "No file descriptors became ready within " << timeout_sec << " seconds\n";
    } else {
        if (FD_ISSET(fd, &read_set)) {
            std::cout << "Input is ready to be read\n";
            // Read from the file descriptor here
        }
    }

    return 0;
}