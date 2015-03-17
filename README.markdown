<h1 id="задание-первое">Задание первое</h1>
<p>В первом задании необходимо реализовать примитивный вариант утилиты <code>cat</code>, копирующий символы из stdin на stdout.</p>
<h2 id="первая-часть">Первая часть</h2>
<p>Требуется реализовать динамическую библиотеку, содержащую функции-хелперы <code>read_</code> и <code>write_</code>, делающие то же, что и <code>read</code> и <code>write</code>, но для буфера целиком (либо до EOF). Сигнатуры хелперов должны совпадать с сигнатурами оригинальных функций.</p>
<h3 id="файлы-в-репозитории">Файлы в репозитории</h3>
<ul>
<li><code>/lib/helpers.h</code></li>
<li><code>/lib/helpers.c</code></li>
<li><code>/lib/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется">Скриптом сборки генерируется</h3>
<ul>
<li><code>/lib/libhelpers.so</code></li>
</ul>
<h3 id="hints">Hints</h3>
<ul>
<li><code>man 2 read</code></li>
<li><code>man 2 write</code></li>
</ul>
<h2 id="вторая-часть">Вторая часть</h2>
<p>Используя функции-хелперы из первой части, реализовать утилиту <code>cat</code>. В качестве аргумента <code>fd</code> хелперам <code>read_</code> и <code>write_</code> необходимо передавать <code>STDIN_FILENO</code> и <code>STDOUT_FILENO</code> соответственно.</p>
<h3 id="файлы-в-репозитории-1">Файлы в репозитории</h3>
<ul>
<li><code>/cat/cat.c</code></li>
<li><code>/cat/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется-1">Скриптом сборки генерируется</h3>
<ul>
<li><code>/cat/cat</code></li>
</ul>
<h3 id="пример-работы">Пример работы</h3>
<ul>
<li><code>./cat &lt; cat.c &gt; cat2.c &amp;&amp; diff cat.c cat2.c &amp;&amp; echo OK</code></li>
</ul>
<h2 id="дедлайн">Дедлайн</h2>
<ul>
<li><code>11 марта, 06:00 (GMT+3)</code></li>
</ul>
<h1 id="задание-второе">Задание второе</h1>
<p>Требуется реализовать утилиту <code>revwords</code>, читающую слова из stdin и выводящую в stdout эти же слова развёрнутыми. Гарантируется, что каждое слово имеет длину не более 4096 байт.</p>
<h2 id="первая-часть-1">Первая часть</h2>
<p>Добавить в библиотеку функцию <code>read_until</code>, имеющую следующую сигнатуру:</p>
<pre><code>ssize_t read_until(int fd, void * buf, size_t count, char delimiter);</code></pre>
<p>Функция имеет ту же семантику, что и <code>read_</code> из первого задания, с одним отличием: она прекращает считывание из <code>fd</code> не только при заполнении буфера, но и при наличии символа <code>delimiter</code> в уже заполенной части буфера.</p>
<h3 id="файлы-в-репозитории-2">Файлы в репозитории</h3>
<ul>
<li><code>/lib/helpers.h</code></li>
<li><code>/lib/helpers.c</code></li>
<li><code>/lib/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется-2">Скриптом сборки генерируется</h3>
<ul>
<li><code>/lib/libhelpers.so</code></li>
</ul>
<h2 id="вторая-часть-1">Вторая часть</h2>
<p>Используя функции из библиотеки, реализовать требуемую утилиту. Если в буфере есть слово, которое может быть выведено на stdout, утилита не должна ожидать данных из stdin.</p>
<p>Слова отделяются друг от друга пробелом. Все прочие символы, в том числе <code>\n</code> и <code>\t</code>, считаются буквой.</p>
<h3 id="файлы-в-репозитории-3">Файлы в репозитории</h3>
<ul>
<li><code>/revwords/revwords.c</code></li>
<li><code>/revwords/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется-3">Скриптом сборки генерируется</h3>
<ul>
<li><code>/revwords/revwords</code></li>
</ul>
<h3 id="пример-использования">Пример использования</h3>
<p>Команда <code>(echo -ne &quot;abc def\ngh&quot; ; sleep 3; echo -ne &quot; qwer&quot;) | ./revwords</code> должна вести себя следующим образом: вывести <code>cba␣</code>, три секунды ничего не делать, вывести <code>hg\nfed␣rewq</code>, где знаком <code>␣</code> обозначается пробел. <!-- Бесит pandoc, сжирающий пробелы в конце verbatim-блока --></p>
<h2 id="дедлайн-1">Дедлайн</h2>
<ul>
<li><code>18 марта, 06:00 (GMT+3)</code></li>
</ul>
<h1 id="задание-третье">Задание третье</h1>
<p>Реализовать утилиту <code>filter</code>, читающую строки из stdin, передающую эти строки как последний аргумент команде, указанной в <code>argv</code>, и выводящую только те из них, на которых команда завершилась с нулевым кодом возврата.</p>
<h2 id="часть-первая">Часть первая</h2>
<p>Добавить в библиотеку следующую функцию:</p>
<p><code>int spawn(const char * file, char * const argv [])</code></p>
<p>Функция должна запускать исполняемый файл <code>file</code>, выбираемый в соответствии с переменной окружения <a href="http://en.wikipedia.org/wiki/PATH_(variable)"><code>PATH</code></a>, с аргументами, задаваемыми в <code>argv</code>, дожидаться её завершения и возвращать её код возврата.</p>
<p>Например, <del>если специально не предпринимать против этого мер,</del> следующий код</p>
<pre><code>char* args[] = {&quot;ls&quot;, &quot;/bin&quot;, NULL};
int res = spawn(&quot;ls&quot;, args);</code></pre>
<p>должен показывать содержимое директории <code>/bin</code> и возвращать <code>res = 0</code>.</p>
<h3 id="файлы-в-репозитории-4">Файлы в репозитории</h3>
<ul>
<li><code>/lib/helpers.h</code></li>
<li><code>/lib/helpers.c</code></li>
<li><code>/lib/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется-4">Скриптом сборки генерируется</h3>
<ul>
<li><code>/lib/libhelpers.so</code></li>
</ul>
<h3 id="hints-1">Hints</h3>
<ul>
<li><code>man 2 fork</code></li>
<li><code>man 3 exec</code></li>
<li><code>man 2 wait</code></li>
</ul>
<h3 id="запрещается-использовать">Запрещается использовать</h3>
<ul>
<li><code>man 3 system</code></li>
</ul>
<h2 id="часть-вторая">Часть вторая</h2>
<p>Используя библиотеку, реализовать требуемую утилиту.</p>
<p>Строки отделяются друг от друга <code>\n</code>.</p>
<h3 id="файлы-в-репозитории-5">Файлы в репозитории</h3>
<ul>
<li><code>/filter/filter.h</code></li>
<li><code>/filter/filter.c</code></li>
<li><code>/filter/Makefile</code></li>
</ul>
<h3 id="скриптом-сборки-генерируется-5">Скриптом сборки генерируется</h3>
<ul>
<li><code>/filter/filter</code></li>
</ul>
<h3 id="пример-использования-1">Пример использования</h3>
<ul>
<li>Вызов <code>echo -ne &quot;/bin/sh\n/blablabla\n/bin/cat\n&quot; | ./filter tar cf   /tmp/filter.tar</code> должен вывести <code>/bin/sh</code> и <code>/bin/cat</code>.</li>
</ul>
<h2 id="дедлайн-2">Дедлайн</h2>
<ul>
<li><code>25 марта, 06:00 (GMT+3)</code></li>
</ul>