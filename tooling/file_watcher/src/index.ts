import { Command } from 'commander';
import { start_watching } from './bombastic_side_eye';

const program = new Command();
let exclude_patterns: RegExp[] = [];


// gracefully handle ctrl+c
process.on('SIGINT', () => {
    console.log('\x1b[31m', 'Caught interrupt signal, exiting gracefully\x1b[0m');
    process.exit(0);
});

program
    .name('My File Watcher')
    .version('0.0.1')
    .description('Watch for file changes and reload a C++ application');

program
    .command('watch <directory_to_watch>')
    .description('Watch for file changes in the specified directory')
    .option('-e, --exclude <exclude_patterns...>', 'Exclude files matching the specified patterns')
    .option('--capture-stdout', 'Capture stdout from the child process and print it to the console')
    .option('--capture-stderr', 'Capture stderr from the child process and print it to the console')
    .option('--capture-stdin', 'Capture stdin from the child process and print it to the console')
    .option('--capture-all', 'Capture all stdio from the child process and print it to the console')
    .option('-c --command <command_to_run>', 'Command to run when a file changes')
    .option('-t --timeout <timeout_in_seconds>', 'Timeout in seconds for the child process to start')
    .option('-d --delay <delay_in_seconds>', 'Delay in seconds before restarting the child process')
    .option('-r --restarts <max_restarts>', 'Maximum number of restarts before giving up')
    .option('-s --signal <signal>', 'Signal to send to the child process when restarting')
    .option('-v --verbose', 'Verbose output')
    .option('-q --quiet', 'Quiet output')
    .option('--fire-on-start', 'Fire the command when the watcher starts')
    .action((directory_to_watch: string, options: any) => {
        if (!options.command) throw new Error('Command to run is required');
        exclude_patterns = options?.exclude?.map((pattern: string) => new RegExp(pattern));
        start_watching(directory_to_watch, options.command, exclude_patterns, options);
    });


console.log(program.opts());


program.parse(process.argv);