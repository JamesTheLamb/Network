-module(tcp).

-export([server/0, chat_router/1, client_handler/2]).

server() ->
    {ok, LSock} = gen_tcp:listen(4300, [binary, {packet, 0}, 
                                        {active, false}, 
                                        {reuseaddr, true}]),
    register(chat_router, spawn(tcp, chat_router, [dict:new()])),

    accept_loop(LSock).
    
accept_loop(LSock) ->
    {ok, Sock} = gen_tcp:accept(LSock),
    spawn(tcp, client_handler, [Sock, 0]),
    accept_loop(LSock).

%do_recv(Sock) ->
%    case gen_tcp:recv(Sock, 0) of
%        {ok, B} ->
%	    io:format("Received ~s", [B]),
%	    spawn(tcp, client_handler, [Sock]),
%            do_recv(Sock);
%        {error, Reason} ->
%            io:format("error recv: ~s", [Reason])
%    end.
    
    
%%%%%%%%%%%%%%%
% Chat Router %
%%%%%%%%%%%%%%%

chat_router(Clients) ->
    receive
        {register, Name, Socket} ->
            case dict:find(Name, Clients) of
                {ok,_} ->
		    gen_tcp:send(Socket, <<"Name already registered, try again with a different name.">>),
                    chat_router(Clients);
                error->
                    gen_tcp:send(Socket, <<"Registered!">>),
                    chat_router(dict:store(Name, {Socket, 0, 0}, Clients))
            end;
            
        {get_pos, {Socket, PosX, PosY}, ClientName} ->
            case dict:find(ClientName, Clients) of
                {ok, {Socket, _, _}} ->
                    gen_tcp:send(Socket, term_to_binary(ClientName ++ " X: " ++ Socket ++ " Y: " ++ Socket)),
                    chat_router(dict:store(ClientName, {Socket, PosX, PosY}, Clients));
                error->
                    io:format("Error!"),
                    chat_router(Clients)
            end;
            
        update_position ->
            chat_router(Clients);
            
        {greet_all, Sender, Msg} ->
            %F = fun(ClientName, {Socket, _, _}) -> gen_tcp:send(Socket, term_to_binary({ClientName ++ " sent '" ++ Msg ++ "'"})) end,
            F = fun(ClientName, {Socket, _, _}) -> gen_tcp:send(Socket, <<Sender/binary, <<" sendt '">>/binary, Msg/binary, <<"'">>/binary>>) end,
            dict:map(F, Clients),
            chat_router(Clients)
            
        
    end.
    
    
%%%%%%%%%%%%%%%%%%
% Client Handler %
%%%%%%%%%%%%%%%%%%
client_handler(Socket, ClientName) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Bin} ->
            [Cmd|Remainder] = binary:split(Bin, <<":">>, [global]),
            io:format("Command '~p' received.~n", [Cmd]),
        case Cmd of
	    <<"reg">> ->
		[Name|_] = Remainder,
                chat_router ! {register, Name, Socket},
                client_handler(Socket, Name);
            <<"greet_all">> ->
		[Msg|_] = Remainder,
                chat_router ! {greet_all, ClientName, Msg},
                client_handler(Socket, ClientName);
	    <<"get_position">> ->
		[ClientName,PosX,PosY|_] = Remainder, %% "get_position:james:123:456:"
                chat_router ! {get_pos, {Socket, PosX, PosY}, ClientName};
	    <<"update_position">> ->
                chat_router ! update_position;
            _ ->
                io:format("Unsupported command.~n", [])
        end,
	client_handler(Socket, ClientName);
    {error, Reason} ->
        io:format("Error: ~s!~n", [Reason])
    end.
    
%%erl -noshell -s tcp server -s init stop
%% compile: erlc tcp.erl











