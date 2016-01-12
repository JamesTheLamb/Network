-module(chat).
-compile(export_all).

-define(Options, [binary, {packet, raw}, {active, false}, {reuseaddr, true}]).

%%%%%%%%%%%%%%%
% Chat Server %
%%%%%%%%%%%%%%%
start_server(Port) ->
    {ok, Listener_Socket} = gen_tcp:listen(Port, ?Options),
    io:format("Starting chat router...~n", []),
    register(chat_router, spawn(chat, chat_router, [dict:new()])),
    accept_loop(Listener_Socket).

accept_loop(Listener_Socket) ->
    io:format("Waiting for connections...~n", []),
    {ok, Socket} = gen_tcp:accept(Listener_Socket),
    io:format("Connection accepted!~n", []),
    spawn(chat, client_handler, [Socket]),
    accept_loop(Listener_Socket).

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
                    chat_router(dict:store(ClientName, {Socket, 0, 0}, Clients))
            end;
        {newpos, ClientName} ->
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

greet(Socket, Msg) ->
    gen_tcp:send(Socket, term_to_binary(Msg)).
    
position(_ClientName, {Socket, _, _}, Data)->
    gen_tcp:send(Socket, term_to_binary(Data)).

%%%%%%%%%%%%%%%%%%
% Client Handler %
%%%%%%%%%%%%%%%%%%
client_handler(Socket) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Bin} ->
            Cmd = binary_to_term(Bin),
            io:format("Command '~p' received.~n", [Cmd]),
        case Cmd of
            {register, ClientName} ->
                chat_router ! {register, ClientName, Socket};
            {greet_all, Msg} ->
                chat_router ! {greet_all, Msg};
            {newpos, ClientName} ->
                chat_router ! {newpos, ClientName};
            update_position ->
                chat_router ! update_position;
            _ ->
                io:format("Unsupported command.~n", [])
        end,
    client_handler(Socket);
    {error, _} ->
        io:format("Lost connection to client!~n", [])
    end.

%%%%%%%%%%%%%%%
% Chat Client %
%%%%%%%%%%%%%%%
front(L) ->
    lists:reverse(tl(lists:reverse(L))).

start_client(Port) ->
    {ok, Socket} = gen_tcp:connect("localhost", Port, ?Options),
    io:format("Client connected to chat server!~n", []),
    spawn_link(chat, client_printer, [Socket]),
    client_process(Socket).

client_printer(Socket) ->
    {ok, Bin} = gen_tcp:recv(Socket, 0),
    io:format("Received ~p.~n", [binary_to_term(Bin)]),
    client_printer(Socket).

client_send(Socket, Term) ->
    gen_tcp:send(Socket, term_to_binary(Term)).

client_process(Socket) ->
    Cmd = io:get_line("Enter command: "),
    [Command | Data] = string:tokens(front(Cmd), " "),
    Atom = list_to_atom(Command),
    MessageThing = string:join(Data, " "),
    gen_tcp:send(Socket, term_to_binary({Atom, MessageThing})),
    client_process(Socket).
