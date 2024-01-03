idf_component_register(SRCS "lftpd.c" "lftpd_inet.c" "lftpd_io.c" "lftpd_log.c" "lftpd_string.c" "lftpd_mgr.c"
                    INCLUDE_DIRS "include" 
                    PRIV_INCLUDE_DIRS "private"
                    REQUIRES log)