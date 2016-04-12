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
		
	{get_all_player, Socket} ->
		gen_tcp:send(Socket, integer_to_list(dict:size(Clients))),
		chat_router(Clients);
	    
        {register, Name, X, Y, Socket} ->
            case dict:find(Name, Clients) of
                {ok,_} ->
		    gen_tcp:send(Socket, <<"Name already registered, try again with a different name.">>),
                    chat_router(Clients);
                error->
		    chat_router(dict:store(Name, {Socket, <<X/binary>>, <<Y/binary>>}, Clients))
            end;
            
        {unregister, Name, Socket} ->
	    case dict:find(Name, Clients) of
		{ok, _} ->
		    dict:erase(Name, Clients),
		    chat_router(Clients);
		error->
		    gen_tcp:send(Socket, <<"Name cannot be found to be unregistered.">>),
		    chat_router(Clients)
	    end;

	{player, Name} ->
		F = fun(ClientName, {Socket, _, _}) -> gen_tcp:send(Socket, <<Name/binary, <<" Registered!">>/binary>>) end,
		dict:map(F, Clients),
		chat_router(Clients);
			
            
        {update_position, Direction, ClientName} ->
		F = fun(Client, {Socket, _, _}) -> gen_tcp:send(Socket, <<ClientName/binary, <<" moved ">>/binary, Direction/binary>>) end,
		dict:map(F, Clients),
	   	chat_router(Clients);

            
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
	    <<"get_all_players">> ->
		chat_router ! {get_all_player, Socket},
		client_handler(Socket, ClientName);
	    <<"unregister">> ->
		[Name|_] = Remainder,
		chat_router ! {unregister, Name, Socket},
		client_handler(Socket, Name);
	    <<"reg">> ->
		[Name, X, Y|_] = Remainder,
                chat_router ! {register, Name, X, Y, Socket},
		chat_router ! {player, Name},
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











