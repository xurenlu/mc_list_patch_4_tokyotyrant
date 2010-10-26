static void do_mc_list(TTSOCK *sock, TASKARG *arg, TTREQ *req, char **tokens, int tnum){
    ttservlog(g_serv, TTLOGDEBUG, "doing mc_list command");
    TCADB *adb = arg->adb;
    if(tnum < 1){
        ttsockprintf(sock, "CLIENT_ERROR error\r\n");
        return;
    }
    TCXSTR *xstr = tcxstrnew();
    TCXSTR * head = tcxstrnew();

    pthread_cleanup_push((void (*)(void *))tcxstrdel, xstr);
    pthread_cleanup_push((void (*)(void *))tcxstrdel, head);

    tcadbiterinit(adb);
    char * key;
    int list_size;
    while((key=tcadbiternext2(adb))!=NULL){
        tcxstrcat(xstr, key, strlen(key));
        tcxstrcat(xstr,"\r\n",2);
    }
    list_size=tcxstrsize(xstr);
    tcxstrprintf(head,"LIST %d\r\n",list_size);
    tcxstrcat(xstr, "END\r\n",5);

    if( ttsocksend(sock, tcxstrptr(head), tcxstrsize(head)) && ttsocksend(sock,tcxstrptr(xstr),tcxstrsize(xstr)) ){
        req->keep = true;
    } else {
        ttservlog(g_serv, TTLOGINFO, "do_mc_list: response failed");
        return ;
    }
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}
