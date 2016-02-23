-module(tcp).

-export([server/0, do_recv/1]).

server() ->
    {ok, LSock} = gen_tcp:listen(4300, [binary, {packet, 0}, 
                                        {active, false}, {reuseaddr, true}]),
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
            gen_tcp:send(Sock, B);
        {error, Reason} ->
            io:format("error recv: ~s", [Reason])
    end.
    
%%erl -noshell -s tcp server -s init stop