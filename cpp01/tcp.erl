-module(tcp).

-export([server/0, do_recv/1]).

server() ->
    {ok, LSock} = gen_tcp:listen(4300, [binary, {packet, 0}, 
                                        {active, false}, 
                                        {reuseaddr, true}]),
    accept_loop(LSock).
    
accept_loop(LSock) ->
    {ok, Sock} = gen_tcp:accept(LSock),
    spawn(tcp, do_recv, [Sock]),
    %ok = do_recv(Sock),
    %ok = gen_tcp:close(Sock).
    accept_loop(LSock).

do_recv(Sock) ->
    case gen_tcp:recv(Sock, 0) of
        {ok, B} ->
	    io:format("Received ~s", [B]),
            interpret(B, Sock),
            do_recv(Sock);
        {error, Reason} ->
            io:format("error recv: ~s", [Reason])
    end.
    
    
interpret(Message, Sock) ->
    case Message of
      <<"reg:", Remainder/binary>> ->
	[Name|_] = binary:split(Remainder, <<":">>),
	message_router ! {register_nick, Name, Sock};
      <<"quit:", _/binary>> ->
	message_router ! {unregister_nick, Name},
	gen_tcp:close(Sock);
      _ -> 
    end.
    
%%%%%%%%%%%%%%%
% Chat Router %
%%%%%%%%%%%%%%%

chat_router(Clients) ->
    receive
        {register, ClientName, Socket} ->
            case dict:find(ClientName, Clients) of
                {ok,_} ->
                    chat_router(Clients);
                error->
                    gen_tcp:send(Socket, term_to_binary("Registered!")),
                    chat_router(dict:store(ClientName, {Socket, posX, posY}, Clients))
            end;
        {newpos, {Socket, posX, posY}, ClientName} ->
            case dict:find(ClientName, Clients) of
                {ok, {Socket, _, _}} ->
                    gen_tcp:send(Socket, term_to_binary("Increased " ++ ClientName ++ " X and Y by 1")),
                    chat_router(dict:store(ClientName, {Socket, 1, 1}, Clients));
                error->
                    io:format("Error!"),
                    chat_router(Clients)
            end;
        update_position ->
            chat_router(Clients);
        {greet_all, Msg} ->
            F = fun(ClientName, {Socket, _, _}) -> gen_tcp:send(Socket, term_to_binary({ClientName ++ " sent '" ++ Msg ++ "'"})) end,
            dict:map(F, Clients),
            chat_router(Clients)
    end.
    
%%erl -noshell -s tcp server -s init stop
%% compile: erlc tcp.erl