/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "QUECTEL/BG96/QUECTEL_BG96_CellularStack.h"
#include "CellularLog.h"

using namespace mbed;

QUECTEL_BG96_CellularStack::QUECTEL_BG96_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
#ifdef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
    , _dns_callback(NULL), _dns_version(NSAPI_UNSPEC)
#endif
{
    _at.set_urc_handler("+QIURC: \"recv", mbed::Callback<void()>(this, &QUECTEL_BG96_CellularStack::urc_qiurc_recv));
    _at.set_urc_handler("+QIURC: \"close", mbed::Callback<void()>(this, &QUECTEL_BG96_CellularStack::urc_qiurc_closed));
#ifdef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
    _at.set_urc_handler("+QIURC: \"dnsgip\",", mbed::Callback<void()>(this, &QUECTEL_BG96_CellularStack::urc_qiurc_dnsgip));
#endif
}

QUECTEL_BG96_CellularStack::~QUECTEL_BG96_CellularStack()
{
}

nsapi_error_t QUECTEL_BG96_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_BG96_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_BG96_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &address)
{
    CellularSocket *socket = (CellularSocket *)handle;

    int modem_connect_id = -1;
    int err = -1;

    int request_connect_id = find_socket_index(socket);
    // assert here as its a programming error if the socket container doesn't contain
    // specified handle
    MBED_ASSERT(request_connect_id != -1);

    _at.lock();
    if (socket->proto == NSAPI_TCP) {
        char ipdot[NSAPI_IP_SIZE];
        ip2dot(address, ipdot);
        _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d%d%d", _cid, request_connect_id, "TCP",
                           ipdot, address.get_port(), socket->localAddress.get_port(), 0);

        handle_open_socket_response(modem_connect_id, err);

        if ((_at.get_last_error() == NSAPI_ERROR_OK) && err) {
            if (err == BG96_SOCKET_BIND_FAIL) {
                socket->id = -1;
                _at.unlock();
                return NSAPI_ERROR_PARAMETER;
            }
            _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);

            _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d%d%d", _cid, request_connect_id, "TCP",
                               ipdot, address.get_port(), socket->localAddress.get_port(), 0);

            handle_open_socket_response(modem_connect_id, err);
        }
    }

    // If opened successfully BUT not requested one, close it
    if (!err && (modem_connect_id != request_connect_id)) {
        _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);
    }

    nsapi_error_t ret_val = _at.get_last_error();
    _at.unlock();

    if ((ret_val == NSAPI_ERROR_OK) && (modem_connect_id == request_connect_id)) {
        socket->id = request_connect_id;
        socket->remoteAddress = address;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

void QUECTEL_BG96_CellularStack::urc_qiurc_recv()
{
    urc_qiurc(URC_RECV);
}

void QUECTEL_BG96_CellularStack::urc_qiurc_closed()
{
    urc_qiurc(URC_CLOSED);
}

#ifdef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
bool QUECTEL_BG96_CellularStack::read_dnsgip(SocketAddress &address, nsapi_version_t _dns_version)
{
    if (_at.read_int() == 0) {
        int count = _at.read_int();
        _at.skip_param();
        for (; count > 0; count--) {
            _at.resp_start("+QIURC: \"dnsgip\",");
            char ipAddress[NSAPI_IP_SIZE];
            _at.read_string(ipAddress, sizeof(ipAddress));
            if (address.set_ip_address(ipAddress)) {
                if (_dns_version == NSAPI_UNSPEC || _dns_version == address.get_ip_version()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void QUECTEL_BG96_CellularStack::urc_qiurc_dnsgip()
{
    if (!_dns_callback) {
        return;
    }
    SocketAddress address;
    if (read_dnsgip(address, _dns_version)) {
        _dns_callback(NSAPI_ERROR_OK, &address);
    } else {
        _dns_callback(NSAPI_ERROR_DNS_FAILURE, NULL);
    }
    _dns_callback = NULL;
}
#endif

void QUECTEL_BG96_CellularStack::urc_qiurc(urc_type_t urc_type)
{
    _at.lock();
    _at.skip_param();
    const int sock_id = _at.read_int();
    const nsapi_error_t err = _at.unlock_return_error();

    if (err != NSAPI_ERROR_OK) {
        return;
    }

    CellularSocket *sock = find_socket(sock_id);
    if (sock) {
        if (urc_type == URC_CLOSED) {
            tr_info("Socket closed %d", sock_id);
            sock->closed = true;
        }
        if (sock->_cb) {
            sock->_cb(sock->_data);
        }
    }
}

int QUECTEL_BG96_CellularStack::get_max_socket_count()
{
    return BG96_SOCKET_MAX;
}

bool QUECTEL_BG96_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP || protocol == NSAPI_TCP);
}

nsapi_error_t QUECTEL_BG96_CellularStack::socket_close_impl(int sock_id)
{
    _at.set_at_timeout(BG96_CLOSE_SOCKET_TIMEOUT);
    nsapi_error_t err = _at.at_cmd_discard("+QICLOSE", "=", "%d", sock_id);
    _at.restore_at_timeout();

    return err;
}

void QUECTEL_BG96_CellularStack::handle_open_socket_response(int &modem_connect_id, int &err)
{
    // OK
    // QIOPEN -> should be handled as URC?
    _at.set_at_timeout(BG96_CREATE_SOCKET_TIMEOUT);
    _at.resp_start("+QIOPEN:");
    _at.restore_at_timeout();
    modem_connect_id = _at.read_int();
    err = _at.read_int();
}

nsapi_error_t QUECTEL_BG96_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int modem_connect_id = -1;
    int remote_port = 0;
    int err = -1;

    int request_connect_id = find_socket_index(socket);
    // assert here as its a programming error if the socket container doesn't contain
    // specified handle
    MBED_ASSERT(request_connect_id != -1);

    if (socket->proto == NSAPI_UDP && !socket->connected) {
        _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d%d%d", _cid, request_connect_id, "UDP SERVICE",
                           (_ip_ver_sendto == NSAPI_IPv4) ? "127.0.0.1" : "0:0:0:0:0:0:0:1",
                           remote_port, socket->localAddress.get_port(), 0);

        handle_open_socket_response(modem_connect_id, err);

        if ((_at.get_last_error() == NSAPI_ERROR_OK) && err) {
            if (err == BG96_SOCKET_BIND_FAIL) {
                socket->id = -1;
                return NSAPI_ERROR_PARAMETER;
            }
            socket_close_impl(modem_connect_id);

            _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d%d%d", _cid, request_connect_id, "UDP SERVICE",
                               (_ip_ver_sendto == NSAPI_IPv4) ? "127.0.0.1" : "0:0:0:0:0:0:0:1",
                               remote_port, socket->localAddress.get_port(), 0);

            handle_open_socket_response(modem_connect_id, err);
        }
    } else if (socket->proto == NSAPI_UDP && socket->connected) {
        char ipdot[NSAPI_IP_SIZE];
        ip2dot(socket->remoteAddress, ipdot);
        _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d", _cid, request_connect_id, "UDP",
                           ipdot, socket->remoteAddress.get_port());

        handle_open_socket_response(modem_connect_id, err);

        if ((_at.get_last_error() == NSAPI_ERROR_OK) && err) {
            if (err == BG96_SOCKET_BIND_FAIL) {
                socket->id = -1;
                return NSAPI_ERROR_PARAMETER;
            }
            socket_close_impl(modem_connect_id);

            _at.at_cmd_discard("+QIOPEN", "=", "%d%d%s%s%d", _cid, request_connect_id, "UDP",
                               ipdot, socket->remoteAddress.get_port());

            handle_open_socket_response(modem_connect_id, err);
        }
    }

    // If opened successfully BUT not requested one, close it
    if (!err && (modem_connect_id != request_connect_id)) {
        socket_close_impl(modem_connect_id);
    }

    nsapi_error_t ret_val = _at.get_last_error();

    if (ret_val == NSAPI_ERROR_OK && (modem_connect_id == request_connect_id)) {
        socket->id = request_connect_id;
    }

    return ret_val;
}

nsapi_size_or_error_t QUECTEL_BG96_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                     const void *data, nsapi_size_t size)
{
    if (size > BG96_MAX_SEND_SIZE) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (_ip_ver_sendto != address.get_ip_version()) {
        _ip_ver_sendto =  address.get_ip_version();
        socket_close_impl(socket->id);
        create_socket_impl(socket);
    }

    int sent_len = 0;
    int sent_len_before = 0;
    int sent_len_after = 0;

    // Get the sent count before sending
    _at.at_cmd_int("+QISEND", "=", sent_len_before, "%d%d", socket->id, 0);

    // Send
    if (socket->proto == NSAPI_UDP) {
        char ipdot[NSAPI_IP_SIZE];
        ip2dot(address, ipdot);
        _at.cmd_start_stop("+QISEND", "=", "%d%d%s%d", socket->id, size,
                           ipdot, address.get_port());
    } else {
        _at.cmd_start_stop("+QISEND", "=", "%d%d", socket->id, size);
    }

    _at.resp_start(">");
    _at.write_bytes((uint8_t *)data, size);
    _at.resp_start();
    _at.set_stop_tag("\r\n");
    // Possible responses are SEND OK, SEND FAIL or ERROR.
    char response[16];
    response[0] = '\0';
    _at.read_string(response, sizeof(response));
    _at.resp_stop();
    if (strcmp(response, "SEND OK") != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    // Get the sent count after sending
    nsapi_size_or_error_t err = _at.at_cmd_int("+QISEND", "=", sent_len_after, "%d%d", socket->id, 0);
    if (err == NSAPI_ERROR_OK) {
        sent_len = sent_len_after - sent_len_before;
        return sent_len;
    }

    return err;
}

nsapi_size_or_error_t QUECTEL_BG96_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                       void *buffer, nsapi_size_t size)
{
    nsapi_size_or_error_t recv_len = 0;
    int port;
    char ip_address[NSAPI_IP_SIZE + 1];

    if (socket->proto == NSAPI_TCP) {
        // do not read more than max size
        size = size > BG96_MAX_RECV_SIZE ? BG96_MAX_RECV_SIZE : size;
        _at.cmd_start_stop("+QIRD", "=", "%d%d", socket->id, size);
    } else {
        _at.cmd_start_stop("+QIRD", "=", "%d", socket->id);
    }

    _at.resp_start("+QIRD:");
    recv_len = _at.read_int();
    _at.read_string(ip_address, sizeof(ip_address));
    port = _at.read_int();
    if (recv_len > 0) {
        // do not read more than buffer size
        recv_len = recv_len > (nsapi_size_or_error_t)size ? size : recv_len;
        _at.read_bytes((uint8_t *)buffer, recv_len);
    }
    _at.resp_stop();

    // We block only if 0 recv length really means no data.
    // If 0 is followed by ip address and port can be an UDP 0 length packet
    if (!recv_len && port < 0) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (address) {
        address->set_ip_address(ip_address);
        address->set_port(port);
    }

    return recv_len;
}

#ifdef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
nsapi_error_t QUECTEL_BG96_CellularStack::gethostbyname(const char *host, SocketAddress *address,
                                                        nsapi_version_t version, const char *interface_name)
{
    (void) interface_name;
    MBED_ASSERT(host);
    MBED_ASSERT(address);

    _at.lock();

    if (_dns_callback) {
        _at.unlock();
        return NSAPI_ERROR_BUSY;
    }

    if (!address->set_ip_address(host)) {
        _at.set_at_timeout(60 * 1000); // from BG96_TCP/IP_AT_Commands_Manual_V1.0
        _at.at_cmd_discard("+QIDNSGIP", "=", "%d%s", _cid, host);
        _at.resp_start("+QIURC: \"dnsgip\",");
        _at.restore_at_timeout();
        if (!read_dnsgip(*address, version)) {
            _at.unlock();
            return NSAPI_ERROR_DNS_FAILURE;
        }
    }

    return _at.unlock_return_error();
}

nsapi_value_or_error_t QUECTEL_BG96_CellularStack::gethostbyname_async(const char *host, hostbyname_cb_t callback,
                                                                       nsapi_version_t version, const char *interface_name)
{
    (void) interface_name;
    MBED_ASSERT(host);
    MBED_ASSERT(callback);

    _at.lock();

    if (_dns_callback) {
        _at.unlock();
        return NSAPI_ERROR_BUSY;
    }

    _at.at_cmd_discard("+QIDNSGIP", "=", "%d%s", _cid, host);
    if (!_at.get_last_error()) {
        _dns_callback = callback;
        _dns_version = version;
    }

    return _at.unlock_return_error() ? NSAPI_ERROR_DNS_FAILURE : NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BG96_CellularStack::gethostbyname_async_cancel(int id)
{
    _at.lock();
    _dns_callback = NULL;
    _at.unlock();
    return NSAPI_ERROR_OK;
}
#endif

void QUECTEL_BG96_CellularStack::ip2dot(const SocketAddress &ip, char *dot)
{
    if (ip.get_ip_version() == NSAPI_IPv6) {
        const uint8_t *bytes = (uint8_t *)ip.get_ip_bytes();
        for (int i = 0; i < NSAPI_IPv6_BYTES; i += 2) {
            if (i != 0) {
                *dot++ = ':';
            }
            dot += sprintf(dot, "%x", (*(bytes + i) << 8 | *(bytes + i + 1)));
        }
    } else if (ip.get_ip_version() == NSAPI_IPv4) {
        strcpy(dot, ip.get_ip_address());
    } else {
        *dot = '\0';
    }
}
