-module(tcp).

-export([server/0]).

server() ->
    {ok, LSock} = gen_tcp:listen(4300, [binary, {packet, 0}, 
                                        {active, false}, {reuseaddr, true}]),
    {ok, Sock} = gen_tcp:accept(LSock),
    ok = do_recv(Sock),
    ok = gen_tcp:close(Sock).

do_recv(Sock) ->
    case gen_tcp:recv(Sock, 0) of
        {ok, B} ->
	    io:format("Received ~s", [B]),
            gen_tcp:send(Sock, B);
        {error, Reason} ->
            io:format("error recv: ~s", [Reason])
    end.
    
//erl -noshell -s tcp server -s init stop