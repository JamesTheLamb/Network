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
    
    
%%%%%%%%%%%%%%%
% Chat Router %
%%%%%%%%%%%%%%%

chat_router(Clients) ->
    receive
	{login, Socket} ->
	    gen_tcp:send(Socket, <<"Time to register!">>),
	    chat_router(Clients);
	    
        {register, Name, X, Y, Socket} ->
            case dict:find(Name, Clients) of
                {ok,_} ->
		    gen_tcp:send(Socket, <<"Name already registered, try again with a different name.">>),
                    chat_router(Clients);
                error->
                    gen_tcp:send(Socket, <<"Registered!">>),
                    chat_router(dict:store(Name, {Socket, <<X/binary>>, Y}, Clients))
            end;
            
        {update_position, Direction, ClientName} ->
		F = fun(Client, {Socket, _, _}) -> gen_tcp:send(Socket, <<ClientName/binary, <<" moved ">>/binary, Direction/binary>>) end,
		dict:map(F, Clients),
	    case dict:find(ClientName, Clients) of
		{ok, {Socket, PosX, PosY}} ->
		    case Direction of
			<<"left">> ->
 			    PosX2 = list_to_integer(binary_to_list(PosX)) - 40,
 			    PosX3 = integer_to_list(PosX2),
 			    io:format("~p X:~p Y:~p~n", [ClientName, PosX3, PosY]),
 			    gen_tcp:send(Socket, << <<"X ">>/binary>>),
     			    gen_tcp:send(Socket, PosX3),
			    chat_router(dict:store(ClientName, {Socket, list_to_binary(PosX3), PosY}, Clients));
			<<"right">> ->
 			    PosX2 = list_to_integer(binary_to_list(PosX)) + 40,
 			    PosX3 = integer_to_list(PosX2),
 			    io:format("~p X:~p Y:~p~n", [ClientName, PosX3, PosY]),
 			    gen_tcp:send(Socket, << <<"X ">>/binary>>),
 			    gen_tcp:send(Socket, PosX3),
			    chat_router(dict:store(ClientName, {Socket, list_to_binary(PosX3), PosY}, Clients));
			<<"up">> ->
			    PosY2 = list_to_integer(binary_to_list(PosY)) - 40,
 			    PosY3 = integer_to_list(PosY2),
 			    io:format("~p X:~p Y:~p~n", [ClientName, PosX, PosY3]),
 			    gen_tcp:send(Socket, << <<"Y ">>/binary>>),
 			    gen_tcp:send(Socket, PosY3),
			    chat_router(dict:store(ClientName, {Socket, PosX, list_to_binary(PosY3)}, Clients));
			<<"down">> ->
			    PosY2 = list_to_integer(binary_to_list(PosY)) + 40,
 			    PosY3 = integer_to_list(PosY2),
 			    io:format("~p X:~p Y:~p~n", [ClientName, PosX, PosY3]),
 			    gen_tcp:send(Socket, << <<"Y ">>/binary>>),
 			    gen_tcp:send(Socket, PosY3),
			    chat_router(dict:store(ClientName, {Socket, PosX, list_to_binary(PosY3)}, Clients));
			_ ->
			    io:format("Unsupported direction.~n", [])
		    end;
		error ->
		    io:format("Error in moving"),
		    chat_router(Clients)
	    end;

            
        {greet, Addressee, Sender, Msg} ->
	    case dict:find(Addressee, Clients) of
 		{ok, {Socket, _, _}} ->
		    gen_tcp:send(Socket, <<Sender/binary, <<" sent '">>/binary, Msg/binary, <<"'">>/binary>>),
		    io:format("Sent"),
		    chat_router(Clients);
		error ->
		    io:format("The name isn't correct, please try a valid one."),
		    chat_router(Clients)
	    end;
            
        {greet_all, Sender, Msg} ->
            F = fun(ClientName, {Socket, _, _}) -> gen_tcp:send(Socket, <<Sender/binary, <<" sent '">>/binary, Msg/binary, <<"'">>/binary>>) end,
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
	    <<"Login">> ->
		chat_router ! {login, Socket};
	    <<"reg">> ->
		[Name, X, Y|_] = Remainder,
                chat_router ! {register, Name, X, Y, Socket},
                client_handler(Socket, Name);
            <<"greet">> ->
		[Recipient,Msg|_] = Remainder,
		chat_router ! {greet, Recipient, ClientName, Msg},
		client_handler(Socket, ClientName);
            <<"greet_all">> ->
		[Msg|_] = Remainder,
                chat_router ! {greet_all, ClientName, Msg},
                client_handler(Socket, ClientName);
	    <<"update_position">> ->
		[Direction|_] = Remainder,
                chat_router ! {update_position, Direction, ClientName},
                client_handler(Socket, ClientName);
            _ ->
                io:format("Unsupported command.~n", [])
        end,
	client_handler(Socket, ClientName);
    {error, Reason} ->
        io:format("Error: ~s!~n", [Reason])
    end.
    
%%erl -noshell -s tcp server -s init stop
%% compile: erlc tcp.erl











