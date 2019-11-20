set spell spelllang=ru,en
set tags=./tags,tags
autocmd BufWritePost * call system("ctags -R")
" let &path.="src/include,/usr/include/AL,"
let &path.="./"
