1. Install MSYS2 from the [website](https://www.msys2.org/)
2. Open Terminal by lounching MSYS2 UCRT64 Shell
3. _(Optional)_ Switch to Windows Terminal. [Instructions here](https://www.msys2.org/docs/terminals/)
4. Install C++ compiler - `pacman -S mingw-w64-ucrt-x86_64-gcc`
5. Install NeoVim Editor - `pacman -S mingw-w64-ucrt-x86_64-neovim`
6. _(Optional)_ Configure NeoVim Editor:
   -  Create config file by running this in Terminal - `vim ~/.config/nvim/init.lua`
   -  Paste this code in config file:

```lua
-- Windows Terminal/MSYS2 Compatibility
-- if vim.fn.has('win32') == 1 then
--   vim.opt.shell = 'C:\\msys64\\usr\\bin\\bash.exe'
--   vim.opt.shellcmdflag = '-c'
--   vim.opt.shellquote = '"'
--   vim.opt.shellxquote = '"'
-- end

-- Basic Settings
vim.opt.number = true
vim.opt.relativenumber = true
vim.opt.tabstop = 3
vim.opt.shiftwidth = 3
vim.opt.softtabstop = 3
vim.opt.expandtab = true
vim.opt.autoindent = true
vim.opt.smartindent = true
vim.opt.wrap = false
vim.opt.ignorecase = true
vim.opt.smartcase = true
vim.opt.hlsearch = true
vim.opt.incsearch = true
vim.opt.termguicolors = true
vim.opt.scrolloff = 5
vim.opt.sidescrolloff = 8
vim.opt.mouse = 'a'
-- vim.opt.clipboard = 'unnamedplus'
vim.opt.swapfile = false
vim.opt.backup = false
vim.opt.writebackup = false
vim.opt.autoread = true
vim.opt.foldmethod = 'manual'
vim.opt.foldlevelstart = 99
vim.opt.updatetime = 50
vim.opt.timeoutlen = 300
vim.opt.splitbelow = true
vim.opt.splitright = true
vim.opt.laststatus = 2
vim.opt.showmode = true
vim.opt.showcmd = true
vim.opt.ruler = true
vim.opt.hidden = true
vim.opt.wildmenu = true
vim.opt.title = true

-- Leader key
vim.g.mapleader = " "

-- Bootstrap lazy.nvim
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable",
    lazypath,
  })
end
vim.opt.rtp:prepend(lazypath)

-- Minimal Plugin Setup
require("lazy").setup({
  -- Color scheme
  {
    "catppuccin/nvim",
    name = "catppuccin",
    priority = 1000,
    config = function()
      vim.cmd.colorscheme "catppuccin-macchiato"
    end,
  },

  -- Auto pairs
  {
    "windwp/nvim-autopairs",
    config = function()
      require("nvim-autopairs").setup({})
    end,
  },

  -- Comments
  {
    "numToStr/Comment.nvim",
    config = function()
      require("Comment").setup()
    end,
  },

  -- Better syntax highlighting
  {
    "nvim-treesitter/nvim-treesitter",
    build = ":TSUpdate",
    config = function()
      require("nvim-treesitter.configs").setup({
        ensure_installed = { "c", "cpp", "python" },
        highlight = { enable = true },
        indent = { enable = true },
      })
    end,
  },

  -- Terminal
  {
    "akinsho/toggleterm.nvim",
    config = function()
      require("toggleterm").setup({
        size = 15,
        open_mapping = [[<c-\>]],
        direction = "horizontal",
        shade_terminals = false,
      })
    end,
  },
})

-- Key Mappings
local keymap = vim.keymap.set

-- Basic file operations
keymap("n", "<C-s>", ":w<CR>", { desc = "Save file" })
keymap("i", "<C-s>", "<Esc>:w<CR>a", { desc = "Save in insert mode" })
keymap("n", "<leader>w", ":w<CR>", { desc = "Save file" })
keymap("n", "<leader>q", ":q<CR>", { desc = "Quit" })
keymap("n", "<leader>x", ":x<CR>", { desc = "Save and quit" })

-- Terminal
keymap("t", "<Esc>", "<C-\\><C-n>", { desc = "Exit terminal mode" })

-- Buffer navigation
keymap("n", "<S-l>", ":bnext<CR>", { desc = "Next buffer" })
keymap("n", "<S-h>", ":bprevious<CR>", { desc = "Previous buffer" })

-- Window navigation
keymap("n", "<C-h>", "<C-w>h", { desc = "Go to left window" })
keymap("n", "<C-j>", "<C-w>j", { desc = "Go to lower window" })
keymap("n", "<C-k>", "<C-w>k", { desc = "Go to upper window" })
keymap("n", "<C-l>", "<C-w>l", { desc = "Go to right window" })

-- Better indenting
keymap("v", "<", "<gv", { desc = "Indent left" })
keymap("v", ">", ">gv", { desc = "Indent right" })

-- Move text up and down
keymap("v", "J", ":m '>+1<CR>gv=gv", { desc = "Move text down" })
keymap("v", "K", ":m '<-2<CR>gv=gv", { desc = "Move text up" })

-- C++ Template from file
local function insert_cp_template()
  local template_path = vim.fn.expand('~/.config/nvim/templates/cp_template.cpp')

  -- Read and insert template
  local lines = vim.fn.readfile(template_path)
  vim.api.nvim_put(lines, "l", false, true)
end

keymap("n", "<leader>t", insert_cp_template, { desc = "Insert C++ template" })

-- Terminal wrapper function (adapted from your config)
local function term_wrapper(command)
  vim.cmd('w')

  -- Close existing terminal if open
  local wins = vim.api.nvim_list_wins()
  for _, win in ipairs(wins) do
    local buf = vim.api.nvim_win_get_buf(win)
    if vim.bo[buf].buftype == 'terminal' then
      vim.api.nvim_win_close(win, false)
    end
  end

  -- Open new terminal
  vim.cmd('rightbelow vsplit')
  vim.cmd('terminal')
  local buf = vim.api.nvim_get_current_buf()
  local job_id = vim.b[buf].terminal_job_id
  if job_id then
    vim.cmd('startinsert')
    vim.api.nvim_chan_send(job_id, command .. '\n')
  else
    vim.notify("Failed to create terminal", vim.log.levels.ERROR)
  end
end

-- Compile and run commands
keymap("n", "<F8>", function()
  local file = vim.fn.expand('%')
  local name = vim.fn.expand('%:r')
  local cmd = string.format('g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -Winvalid-pch %s -o %s && ./%s', file, name, name)
  term_wrapper(cmd)
end, { desc = "Quick compile and run" })

keymap("n", "<F5>", function()
  local file = vim.fn.expand('%')
  local name = vim.fn.expand('%:r')
  local cmd = string.format('g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -Winvalid-pch %s -o %s && ./%s < input.txt', file, name, name)
  term_wrapper(cmd)
end, { desc = "Run with input.txt" })

-- Highlight on yank
vim.api.nvim_create_autocmd("TextYankPost", {
  callback = function()
    vim.highlight.on_yank()
  end,
})

-- Enable syntax highlighting
vim.cmd("syntax on")
```

-  save and exit - `:wq`

7. Change the neovim config file location:

-  to check current location run this in terminal `nvim --headless +"echo stdpath('config')" +qa`
-  add this line to the .bashrc file `export XDG_CONFIG_HOME="$HOME/.config"`

8. Create template file for cpp by running this in Terminal - `nvim ~/.config/nvim/templates/cp_template.cpp`
   Than, paste this template code:

```cpp
#include "bits/stdc++.h"
using namespace std;

// Type aliases
using ll = long long;
using ld = long double;
using vi = vector<int>;
using vl = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// Macros
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define each(e, v) for (auto &e : (v))
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define repb(i, n) for (int i = (n) - 1; i >= 0; --i)

// Constants
const int INF = 1e9;
const ll LINF = 1e18;
const int MOD = 1e9 + 7;

// Utility functions
template<class T> inline bool minimize(T &a, const T &b){ return b<a ? (a=b,1):0; }
template<class T> inline bool maximize(T &a, const T &b){ return b>a ? (a=b,1):0; }

// I/O utilities
template<typename T>
void read(T& x) { cin >> x; }

template<typename T>
void read(vector<T>& v) {
    for (auto& x : v) cin >> x;
}

template<typename T, typename... Args>
void read(T& first, Args&... args) {
    read(first);
    read(args...);
}

template<typename T>
void print(const T& x) { cout << x << '\n'; }

template<typename T>
void print(const vector<T>& v) {
    for (int i = 0; i < sz(v); ++i) {
        cout << v[i];
        if (i < sz(v) - 1) cout << ' ';
    }
    cout << '\n';
}

template<typename T, typename... Args>
void print(const T& first, const Args&... args) {
    cout << first;
    ((cout << ' ' << args), ...);
    cout << '\n';
}

// ==============================================

void solve() {
   int n;
   read(n);
   print(n);
}

// ==============================================

int main() {
   ios::sync_with_stdio(false);
   cin.tie(nullptr);

   int t = 1;
   // read(t);

   while (t--) {
      solve();
   }

   return 0;
}
```
