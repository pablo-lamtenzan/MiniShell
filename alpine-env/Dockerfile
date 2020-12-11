FROM alpine

ENV REPO="https://github.com/pablo-lamtenzan/minishell.git"
ENV NAME="minishell"
ENV CC="gcc"
ENV CFLAGS="-g3 -Wall -Wextra"

ENV BUILD_DIR="/build"

RUN apk add --update --no-cache build-base git ncurses-dev valgrind

COPY srcs/entrypoint.sh /entrypoint.sh
COPY srcs/musl.supp /musl.supp

ENTRYPOINT ["sh", "/entrypoint.sh"]

CMD ["$BUILD_DIR/$NAME"]
