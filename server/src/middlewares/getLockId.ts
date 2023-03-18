
import { Request, Response, NextFunction } from 'express';
import RequestWithLockId from '../interfaces/request';

export default function getLockId(req: Request, res: Response, next: NextFunction) {
    (req as RequestWithLockId).lockId = req.query.lockId as string;
    next();
}