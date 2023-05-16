#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main() {
  // Get the Wireguard interface name.
  string interface = "wg0";

  // Check if the interface is up.
  if (system("ip link show " + interface + " | grep UP") != 0) {
    cout << "Wireguard interface is not up." << endl;
    return 1;
  }

  // Get the IP address of the Wireguard interface.
  string ip_address = "";
  if (system("ip addr show " + interface + " | grep inet | awk '{print $2}'") != 0) {
    cout << "Could not get IP address of Wireguard interface." << endl;
    return 1;
  }

  // Create a new process to block all traffic except for traffic to the Wireguard IP address.
  pid_t pid = fork();
  if (pid == 0) {
    // Child process
    system("iptables -A OUTPUT -o lo -j ACCEPT");
    system("iptables -A OUTPUT -d " + ip_address + " -j ACCEPT");
    system("iptables -A OUTPUT -j REJECT");
    exit(0);
  } else {
    // Parent process
    // Wait for the child process to exit.
    wait(NULL);
  }

  return 0;
}
