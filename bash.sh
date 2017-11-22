#!/bin/bash
versoes=(original otimizado)
numeros=(32 33 64 65 128 129 256 257 512 1000 2000)
grupos=(FLOPS_DP FLOPS_AVX L3 L2CACHE)
greptarget=("|      DP MFLOP/s      |" "DP MFLOP" "L3 bandwidth" "L2 miss ratio")

if [ -z $1 ]; then
    for versao in ${versoes[@]}; do
        cd $versao
        make purge
        make
        make clean

        if [ ! -d _bench ]; then
            mkdir _bench
        fi

        if [ -f resultado ]; then
            rm resultado
        fi

        if [ -f tempo ]; then
            rm tempo
        fi

        if [ -f tempo_exec ]; then
            rm tempo_exec
        fi

        TIMEFORMAT='%3R'
        for n in ${numeros[@]}; do
            echo -n `date +"[%H:%M:%S %N]"`
            echo "  inicio  ${versoes[versao]} $n"
            echo -n "$n," >> tempo_exec
            ( time invmat -i 10 -r $n -o "_bench/tout$n" ) 2>  _bench/tempo_exec$n
            echo "`cat _bench/tempo_exec$n | cut -d\n -f 2 | tr '\n' '\0'`" >> tempo_exec
            echo -n `date +"[%H:%M:%S %N]"`
            echo -e "  fim  ${versoes[versao]} $n\n"
        done

        for grupo in `seq 0 3`; do
            echo -e "\nGrupo: ${grupos[grupo]},op1,op2" >> resultado
            echo "Grupo: ${grupos[grupo]},op1,op2" >> tempo
            for n in ${numeros[@]}; do
                echo -n `date +"[%H:%M:%S %N]"`
                echo "  inicio  ${versoes[versao]} ${grupos[grupo]} $n"
                echo -n "$n," >> resultado
                echo -n "$n," >> tempo
                ( likwid-perfctr -f -m -C 0 -g ${grupos[grupo]} invmat -i 10 -r $n -o "_bench/out$n${grupos[grupo]}" ) > _bench/lik$n${grupos[grupo]}
                echo -n "`cat _bench/lik$n${grupos[grupo]} | grep "${greptarget[grupo]}" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g'`" >> resultado
                echo ",`cat _bench/lik$n${grupos[grupo]} | grep "${greptarget[grupo]}" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g'`" >> resultado
                echo -n "`cat _bench/out$n${grupos[grupo]} | grep "op1" | cut -d' ' -f 4 `" >> tempo
                echo ", `cat _bench/out$n${grupos[grupo]} | grep "op2" | cut -d' ' -f 4 `" >> tempo
                echo -n `date +"[%H:%M:%S %N]"`
                echo -e "  fim  ${versoes[versao]} ${grupos[grupo]} $n\n"
            done
        done
        cd ..
    done
else
    if [ "$1" = "clean" ]; then
        for versao in ${versoes[@]}; do
            cd $versao
            rm -rf _bench
            rm -f resultado
            rm -f tempo
            rm -f tempo_exec
            make purge
            cd ..
        done
    fi
fi
