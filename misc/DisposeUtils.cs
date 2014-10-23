namespace Yoheie.DisposeUtils
{
    /// <summary>
    /// Dispose Finalize パターン補助
    /// </summary>
    public struct Disposer
    {
        public delegate void DisposeBaseDelegate(bool disposing);
        public delegate void ReleaseManagedOrUnmanagedDelegate();

        private bool disposed;
        private object disposeObject;
        private DisposeBaseDelegate DisposeBase;
        private ReleaseManagedOrUnmanagedDelegate ReleaseManaged;
        private ReleaseManagedOrUnmanagedDelegate ReleaseUnmanaged;

        /// <summary>
        /// IDisposable を直接継承しているクラス用の Disposer コンストラクタ
        /// </summary>
        /// <param name="obj">IDisposable を直接継承しているクラスのオブジェクト</param>
        /// <param name="releaseManaged">obj のマネージドリソースを解放するメソッド</param>
        /// <param name="releaseUnmanaged">obj Noアンマネージドリソースを解放するメソッド</param>
        public Disposer(
                object obj,
                ReleaseManagedOrUnmanagedDelegate releaseManaged,
                ReleaseManagedOrUnmanagedDelegate releaseUnmanaged
                ) : this(obj, null, releaseManaged, releaseUnmanaged)
        {
        }

        /// <summary>
        /// IDisposable を継承しているクラスの派生クラス用の Disposer コンストラクタ
        /// </summary>
        /// <param name="obj">IDisposable を継承しているクラスの派生クラスのオブジェクト</param>
        /// <param name="disposeBase">obj のベースクラスの Dispose を実行するメソッド</param>
        /// <param name="releaseManaged">obj のマネージドリソースを解放するメソッド</param>
        /// <param name="releaseUnmanaged">obj のアンマネージドリソースを解放するメソッド</param>
        public Disposer(
                object obj,
                DisposeBaseDelegate disposeBase,
                ReleaseManagedOrUnmanagedDelegate releaseManaged,
                ReleaseManagedOrUnmanagedDelegate releaseUnmanaged
                )
        {
            if (obj == null)
            {
                throw new System.ApplicationException();
            }

            disposeObject = obj;
            DisposeBase = disposeBase;
            ReleaseManaged = releaseManaged;
            ReleaseUnmanaged = releaseUnmanaged;

            disposed = false;
        }

        /// <summary>
        /// Dispose 済チェック
        /// </summary>
        public void ThrowExceptionIfDisposed()
        {
            if (disposed)
            {
                throw new System.ObjectDisposedException(disposeObject.GetType().ToString());
            }
        }

        /// <summary>
        /// Dispose Finalize パターン補助メソッド
        /// </summary>
        /// <param name="disposing">Dispose メソッドからの実行時 : true / デストラクタからの実行時 : false</param>
        public void Dispose(bool disposing)
        {
            if (!disposed)
            {
                try
                {
                    if (disposing)
                    {
                        if (ReleaseManaged != null)
                        {
                            ReleaseManaged();
                        }
                    }

                    if (ReleaseUnmanaged != null)
                    {
                        ReleaseUnmanaged();
                    }

                    disposed = true;

#if false
                    if ((DisposeBase == null) && disposing)
                    {
                        System.GC.SuppressFinalize(disposeObject);
                    }
#endif
                }
                finally
                {
                    if (DisposeBase != null)
                    {
                        DisposeBase(disposing);
                    }
                }
            }
        }
    }

    class ExampleBase : System.IDisposable
    {
        // IDisposable を継承するクラスは Disposer クラスのメンバを
        // private に用意する。
        private Disposer disposer;

        public ExampleBase()
        {
            // IDisposable を直接継承するクラスでは、マネージドリソース及び
            // アンマネージドリソースの解放を行う private メソッドを用意して
            // Disposer コンストラクタに渡す。
            disposer = new Disposer(this, ReleaseManaged, ReleaseUnmanaged);
        }

        ~ExampleBase()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
#if true
            System.GC.SuppressFinalize(this);
#endif
        }

        protected virtual void Dispose(bool disposing)
        {
            // Disposer.Dispose(bool disposing) メソッドで Dispose Finalize パターンの
            // 処理が行われる。
            disposer.Dispose(disposing);
        }

        private void ReleaseManaged()
        {
            // ここで ExampleBase クラスのマネージドリソースを解放する。
        }

        private void ReleaseUnmanaged()
        {
            // ここで ExampleBase クラスのアンマネージドリソースを解放する。
        }

        public void ExampleBasePublicMethod()
        {
            // Dispose 済みかチェック
            disposer.ThrowExceptionIfDisposed();

            // ExampleBasePublicMethod の処理
        }
    }

    class ExampleDerived : ExampleBase
    {
        // IDisposable を継承するクラスは Disposer クラスのメンバを
        // private に用意する。
        private Disposer disposer;

        public ExampleDerived()
        {
            // IDisposable を継承するクラスの派生クラスでは、ベースクラスの
            // Dispose 実行、マネージドリソース及びアンマネージドリソースの
            // 解放を行う private メソッドを用意して Disposer コンストラクタに渡す。
            disposer = new Disposer(this, DisposeBase, ReleaseManaged, ReleaseUnmanaged);
        }

        protected override void Dispose(bool disposing)
        {
            // Disposer.Dispose(bool) メソッドで Dispose Finalize パターンの
            // 処理が行われる。
            disposer.Dispose(disposing);
        }

        private void DisposeBase(bool disposing)
        {
            // ベースクラスの Dispose(bool disposing) を実行する。
            base.Dispose(disposing);
        }

        private void ReleaseManaged()
        {
            // ここで ExampleDerived クラスのマネージドリソースを解放する。
        }

        private void ReleaseUnmanaged()
        {
            // ここで ExampleDerived クラスのアンマネージドリソースを解放する。
        }

        public void ExampleDerivedPublicMethod()
        {
            // Dispose 済みかチェック
            disposer.ThrowExceptionIfDisposed();

            // ExampleDerivedPublicMethod の処理
        }
    }
}
