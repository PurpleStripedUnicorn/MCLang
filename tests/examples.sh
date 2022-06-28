
s=0
f=0
out=""

for file in ./examples/*
do
    if [[ ! "$file" =~ invalid ]]; then
        if ./build/main -D $file; then
            out+="\033[0;32mSUCCESS:    \033[0m$file\n"
            s=$((s + 1))
        else
            out+="\033[0;31mFAILURE:    \033[0m$file\n"
            f=$((f + 1))
        fi
    fi
done

echo
printf "$out"
echo

if [[ $f -gt 0 ]];
then
    printf "\033[0;31m$f failures, $s successes\n\033[0m"
    exit 1
else
    printf "$f failures, $s successes\n"
fi

echo